#include "stdafx.h"

#include "ColladaStaticMesh.h"
#include "..\..\EntitySystem\Component\Visual\StaticMesh.h"

ColladaStaticMesh::ColladaStaticMesh()
{
	root = NULL;
	library_visual_scenes = NULL;
	library_animations = NULL;
}

ColladaStaticMesh::~ColladaStaticMesh()
{

}

void ColladaStaticMesh::processVisualScenes( std::vector<TempMesh*>& Meshs )
{
	//Get the <visual_scene> node
	daeElement* visual_scene = library_visual_scenes->getDescendant("visual_scene");

	//Get all the <node>'s for the <visual_scene>
	daeTArray<daeElementRef> nodes = visual_scene->getChildren();

	//Keep track of which nodes to process after morphing meshes pass
	vector<bool> process = vector<bool>();

	//Set each boolean to true at first for each node
	for(unsigned int i = 0; i < nodes.getCount(); i++) 
		process.push_back(true);

	//For each <node>...
	for(unsigned int i = 0; i < nodes.getCount(); i++)
	{
		//Process this node?
		if(!process[i]) continue;

		//Get the name and the type, if they exist
		string Name = nodes[i]->getAttribute("name").c_str();
		string Type = nodes[i]->getAttribute("type").c_str();

		//Skip JOINT node's, only meshes
		if(Type == "JOINT") continue;

		//Get the <instance_controller> node that corresponds to this <node> if it exists
		domInstance_controller* instance_controller = NULL;
		instance_controller = (domInstance_controller*)nodes[i]->getDescendant("instance_controller");

		//If instance_controller exists, then this node is for a skinned mesh, else its a static mesh
		if(instance_controller)
		{
		}
		else
		{
			//Get the <instance_geometry> node that corresponds to this <node>
			domInstance_geometry* instance_geometry = NULL;
			instance_geometry = (domInstance_geometry*)nodes[i]->getDescendant("instance_geometry");

			//If there is no <instance_geometry>, this isn't a static mesh and we will skip it.
			if(!instance_geometry) continue;

			//Get the <geometry> node that is referenced by the <instance_geometry>
			daeElement* geometry = instance_geometry->getUrl().getElement();

			//If the referenced node was not found, skip this node
			if(!geometry) continue;

			//Now create a new TempMesh, set it's <geometry> node and get it's World transform.
			Meshs.push_back(new TempMesh( Name, processMatrix(nodes[i]->getDescendant("matrix")) ) );
			Meshs.back()->geometry = geometry;
		}
	}
}

void ColladaStaticMesh::Load( std::string filename , EntitySystem* pEntitySystem )
{
	root = dae.open("..//asset//StaticExample.dae");

	if(!root)
	{
		//cout << "Document import failed. \n";
		return;
	}

	//Get the library_visual_scenes
	library_visual_scenes = root->getDescendant("library_visual_scenes");
	//Check if there is a <library_visual_scenes>
	if(!library_visual_scenes)
	{
		//cout << "<library_visual_scenes> not found.\n";
		return;
	}

	library_animations = root->getDescendant("library_animations");
	//Check if there is a <library_animations>
	if(!library_animations) 
		//cout << "<library_animations> not found.\n";

	//Process <library_visual_scenes>
	processVisualScenes( mesh_container_t );

	processGeometries( mesh_container_t );

	//Compile vertex components into one buffer
	for(unsigned int i = 0; i < mesh_container_t.size(); i++) 
		mesh_container_t[i]->combineComponents();

	MakeTempMeshToMeshComponent( mesh_container_t , pEntitySystem );


	//Set the pointers back to NULL, safety precaution for Debug build
	root = NULL; library_visual_scenes = NULL; library_geometries = NULL;
	dae.close(filename);
}

Matrix44 ColladaStaticMesh::processMatrix( daeElement* matrix )
{
	Matrix44 out;
	std::string world = matrix->getCharData();
	stringstream stm(world);

#pragma region Read the matrix
	stm >> out.m[0][0];
	stm >> out.m[0][1];
	stm >> out.m[0][2];
	stm >> out.m[0][3];

	stm >> out.m[1][0];
	stm >> out.m[1][1];
	stm >> out.m[1][2];
	stm >> out.m[1][3];

	stm >> out.m[2][0];
	stm >> out.m[2][1];
	stm >> out.m[2][2];
	stm >> out.m[2][3];

	stm >> out.m[3][0];
	stm >> out.m[3][1];
	stm >> out.m[3][2];
	stm >> out.m[3][3];
#pragma endregion

	return out;
}

void ColladaStaticMesh::processGeometries( std::vector<TempMesh*>& meshes )
{
	//Foreach mesh...
	for(unsigned int i = 0; i < meshes.size(); i++)
	{
		//Get the <mesh> node
		daeElement* mesh = meshes[i]->geometry->getDescendant("mesh");

		//Get the <source> nodes
		daeTArray<daeElementRef> sources = mesh->getChildren();

		//Get the <triangles> node (yes it will be in the sources array above if you wish to find it that way)
		daeElement* triangles = mesh->getDescendant("triangles");

		//Process each <source> child
		for(unsigned int z = 0; z < sources.getCount(); z++) processSource(meshes[i], sources[z]);

		//Process the <triangles> child
		processTriangles(meshes[i], triangles);
	}
}

void ColladaStaticMesh::processTriangles( TempMesh* mesh, daeElement* triangles )
{
	//Get the <p> node
	daeElement* p = triangles->getDescendant("p");

	//Get the number of faces, multiply by 3 to get number of indices
	unsigned int count = atoi(triangles->getAttribute("count").data()) * 3;

	//Get the raw string representation
	string pArray = p->getCharData();

	//Set up a stringstream to read from the raw array
	stringstream stm(pArray);

	//Read each unsigned int
	for(unsigned int i = 0; i < count; i++)
	{
		unsigned int p = 0;

		stm >> p;

		mesh->Indices.push_back(p);
	}
}

void ColladaStaticMesh::processSource( TempMesh* mesh, daeElement* source )
{
	//Get Positions
	if(source->getAttribute("name").find("position") != string::npos)
	{
		//Get the <float_array> node
		daeElement* float_array = source->getChild("float_array");

		//Get the number of raw float's contained
		unsigned int count = atoi(float_array->getAttribute("count").data());

		//Get the raw string representation
		string positionArray = float_array->getCharData();

		//Set up a stringstream to read from the raw array
		stringstream stm(positionArray);

		//Read each float, in groups of three
		for(unsigned int i = 0; i < (count / 3); i++)
		{
			float x, y, z;

			stm >> x;
			stm >> y;
			stm >> z;

			//Push this back as another Position component
			mesh->Positions.push_back(D3DXVECTOR3(x, y, z));
		}

		return;
	}

	//Get Normals
	if(source->getAttribute("name").find("normal") != string::npos)
	{
		//Get the <float_array> node
		daeElement* float_array = source->getChild("float_array");

		//Get the number of raw float's contained
		unsigned int count = atoi(float_array->getAttribute("count").data());

		//Get the raw string representation
		string normalsArray = float_array->getCharData();

		//Set up a stringstream to read from the raw array
		stringstream stm(normalsArray);

		//Read each float, in groups of three
		for(unsigned int i = 0; i < (count / 3); i++)
		{
			float x, y, z;

			stm >> x;
			stm >> y;
			stm >> z;

			//Push this back as another Position component
			mesh->Normals.push_back(D3DXVECTOR3(x, y, z));
		}

		return;
	}

	//Get UVs at layer0
	if(source->getAttribute("name").find("map1") != string::npos)
	{
		//Get the <float_array> node
		daeElement* float_array = source->getChild("float_array");

		//Get the number of raw float's contained
		unsigned int count = atoi(float_array->getAttribute("count").data());

		//Get the raw string representation
		string uvArray = float_array->getCharData();

		//Set up a stringstream to read from the raw array
		stringstream stm(uvArray);

		//Read each float, in groups of three
		for(unsigned int i = 0; i < (count / 2); i++)
		{
			float x, y;

			stm >> x;
			stm >> y;

			//Push this back as another Position component
			mesh->UVs.push_back(D3DXVECTOR2(x, y));
		}

		return;
	}

	//Get Tangents at layer0, the reason there are different naming schemes, this covers the ones I've come into contact with
	if(source->getAttribute("id").find("map1-tangents") != string::npos || source->getAttribute("id").find("textangents") != string::npos)
	{
		//Get the <float_array> node
		daeElement* float_array = source->getChild("float_array");

		//Get the number of raw float's contained
		unsigned int count = atoi(float_array->getAttribute("count").data());

		//Get the raw string representation
		string tangentsArray = float_array->getCharData();

		//Set up a stringstream to read from the raw array
		stringstream stm(tangentsArray);

		//Read each float, in groups of three
		for(unsigned int i = 0; i < (count / 3); i++)
		{
			float x, y, z;

			stm >> x;
			stm >> y;
			stm >> z;

			//Push this back as another Position component
			mesh->Tangents.push_back(D3DXVECTOR3(x, y, z));
		}

		return;
	}

	//Get BiTangents at layer0, read above about the different names
	if(source->getAttribute("id").find("map1-binormals") != string::npos || source->getAttribute("id").find("texbinormals") != string::npos)
	{
		//Get the <float_array> node
		daeElement* float_array = source->getChild("float_array");

		//Get the number of raw float's contained
		unsigned int count = atoi(float_array->getAttribute("count").data());

		//Get the raw string representation
		string biTangentsArray = float_array->getCharData();

		//Set up a stringstream to read from the raw array
		stringstream stm(biTangentsArray);

		//Read each float, in groups of three
		for(unsigned int i = 0; i < (count / 3); i++)
		{
			float x, y, z;

			stm >> x;
			stm >> y;
			stm >> z;

			//Push this back as another Position component
			mesh->BiTangents.push_back(D3DXVECTOR3(x, y, z));
		}

		return;
	}
}

void ColladaStaticMesh::MakeTempMeshToMeshComponent( std::vector<TempMesh*>& Meshs , EntitySystem* pEntitySystem )
{
	if ( Meshs.empty() )
		return;

	for ( int i=0 ; i < (int)Meshs.size() ; i++ )
	{
		StaticMesh* pStaticMesh = static_cast< StaticMesh*>( pEntitySystem->MakeComponent( IComponent::STATIC_MESH ) );
		Entity* pEntity = pEntitySystem->AllocEntity();
		
		for ( int j=0 ; j< Meshs.at(i)->Positions.size() ; j++ )
		{
			pStaticMesh->Vertices.push_back( Vertex(Meshs.at(i)->Positions[j]
											, Meshs.at(i)->Normals[j]
											, Meshs.at(i)->UVs[j]
											, Meshs.at(i)->Tangents[j]
											, Meshs.at(i)->BiTangents[j] )
												);
		}		

		pStaticMesh->Indices = Meshs.at(i)->Indices;
		pStaticMesh->Name = Meshs.at(i)->Name;

		pEntity->SetMatrix( Meshs.at(i)->World );

		MakeDirectXFriendly( pEntity , pStaticMesh );
		
		pEntity->SetComponent( 1 , pStaticMesh );
	}
}

void ColladaStaticMesh::MakeDirectXFriendly( Entity* pEntity , StaticMesh* pStaticMesh )
{
	Matrix44 Scale; D3DXMatrixScaling(&Scale, 1, 1, -1);

	//Multiply World by Scale
	pEntity->SetMatrix( collada2DirectX(pEntity->GetMatrix()) * Scale );

	//Change Index buffer to render counter clockwise
	for(unsigned int z = 0; z < pStaticMesh->Indices.size(); (z += 3))
	{
		unsigned int v0 = pStaticMesh->Indices[z];
		unsigned int v1 = pStaticMesh->Indices[z+1];
		unsigned int v2 = pStaticMesh->Indices[z+2];

		pStaticMesh->Indices[z] = v0;
		pStaticMesh->Indices[z+1] = v2;
		pStaticMesh->Indices[z+2] = v1;
	}
}

Matrix44 ColladaStaticMesh::collada2DirectX( Matrix44 input )
{
	Matrix44 out;

	out.m[0][0] = input.m[0][0];
	out.m[0][1] = input.m[1][0];
	out.m[0][2] = input.m[2][0];
	out.m[0][3] = input.m[3][0];

	out.m[1][0] = input.m[0][1];
	out.m[1][1] = input.m[1][1];
	out.m[1][2] = input.m[2][1];
	out.m[1][3] = input.m[3][1];

	out.m[2][0] = input.m[0][2];
	out.m[2][1] = input.m[1][2];
	out.m[2][2] = input.m[2][2];
	out.m[2][3] = input.m[3][2];

	out.m[3][0] = input.m[0][3];
	out.m[3][1] = input.m[1][3];
	out.m[3][2] = input.m[2][3];
	out.m[3][3] = input.m[3][3];

	return out;
}
