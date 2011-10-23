#pragma once

#include <dae.h>
#include <dom.h>



#include <vector>

using namespace std;

class ColladaStaticMesh
{
private:
	//Dae file
	DAE dae;

	//Root node
	daeElement* root;
	
	//<library_visual_scenes> node
	daeElement* library_visual_scenes;

	//<library_animations> node
	daeElement* library_animations;

public:
	//Constructor
	ColladaStaticMesh()
	{
		root = NULL;
		library_visual_scenes = NULL;
		library_animations = NULL;
	}

	//Load all the meshes from a file
	MeshManager* Load(string filename)
	{
		//Output object
		MeshManager* Meshes = new MeshManager();

		#pragma region //Open the file and get the root node
		root = dae.open(filename);

		//Check if import succeded
		if(!root)
		{
			cout << "Document import failed. \n";
			return Meshes;
		}
		#pragma endregion
		
		#pragma region //Get the library_visual_scenes
		library_visual_scenes = root->getDescendant("library_visual_scenes");

		//Check if there is a <library_visual_scenes>
		if(!library_visual_scenes)
		{
			cout << "<library_visual_scenes> not found.\n";
			return Meshes;
		}
		#pragma endregion

		#pragma region //Get the library_animations
		library_animations = root->getDescendant("library_animations");

		//Check if there is a <library_animations>
		if(!library_animations) cout << "<library_animations> not found.\n";
		#pragma endregion

		//Process <library_visual_scenes>
		processVisualScenes(Meshes);

		//processSkinnedMeshes
		processSkinnedMeshes(Meshes);

		//Process library_animations if it exists
		if(library_animations) processAnimations(Meshes);

		//Process the <geometry> node for each Static Mesh
		processGeometries(Meshes);
		
		//Compile vertex components into one buffer for each Static Mesh
		for(unsigned int i = 0; i < Meshes->StaticMeshes.size(); i++) Meshes->StaticMeshes[i]->combineComponents();

		//Close the .dae file
		dae.close(filename);

		//Set the pointers back to NULL, safety precaution for Debug build
		root = NULL; library_visual_scenes = NULL; library_animations = NULL;

		//Return MeshManager
		return Meshes;
	}

private:
	//Process a <library_visual_scenes> node
	void processVisualScenes(MeshManager* Meshes)
	{
		//Get the <visual_scene> node
		daeElement* visual_scene = library_visual_scenes->getDescendant("visual_scene");

		//Get all the <node>'s for the <visual_scene>
		daeTArray<daeElementRef> nodes = visual_scene->getChildren();

		//Keep track of which nodes to process after morphing meshes pass
		vector<bool> process = vector<bool>();

		//Set each boolean to true at first for each node
		for(unsigned int i = 0; i < nodes.getCount(); i++) process.push_back(true);

		//Do a first pass for Morphing Meshes
		for(unsigned int i = 0; i < nodes.getCount(); i++)
		{
			//Get the name and the type, if they exist
			string Name = nodes[i]->getAttribute("name").data();
			string Type = nodes[i]->getAttribute("type").data();

			//Skip JOINT node's, only meshes
			if(Type == "JOINT") continue;

			#pragma region //Get <instance_controller>, <controller> and <morph>

			//Get the <instance_controller> node that corresponds to this <node> if it exists
			domInstance_controller* instance_controller = NULL;
			instance_controller = (domInstance_controller*)nodes[i]->getDescendant("instance_controller");

			//Only interested in finding the morphing meshes
			if(!instance_controller) continue;

			//Get the <controller> element
			daeElement* controller = NULL;
			controller = instance_controller->getUrl().getElement();

			//If no <controller> exists that is referenced, skip this
			if(!controller) continue;

			//Does it have a descendant of type <morph>?
			daeElement* morph = NULL;
			morph = controller->getDescendant("morph");

			//if no <morph> then this is a skinned mesh
			if(!morph) continue;

			#pragma endregion

			#pragma region //Get the <source> child of <morph> that contains the Target names

			//Initialize to NULL, debug build safety
			daeElement* targetNameSource = NULL;

			//Get the children of <morph>
			daeTArray<daeElementRef> sources = morph->getChildren();

			//Find from these children, the <source> that contains "targets" in it's id attribute
			for(unsigned int z = 0; z < sources.getCount(); z++)
			{
				//Get ID
				string ID = sources[z]->getAttribute("id").data();

				//Does it contain "targets" in the ID?
				if(ID.find("targets") != string::npos)
				{
					targetNameSource = sources[z];

					break;
				}
			}

			//No targets? Whats the point!
			if(!targetNameSource) continue;

			#pragma endregion

			//Get the names of each geometry node that is a target mesh
			vector<string> targets = processStringSource(targetNameSource->getDescendant("IDREF_array"));

			#pragma region //Get its base geometry

			#pragma region //Get the <geometry> node
			//Get the geometry node reference for this skin
			string geometryReference = morph->getAttribute("source").data();

			//Erase the "#" at the beginning of the name
			geometryReference.erase(0, 1);

			//Get the <geometry> element by searching using the reference
			daeElement* baseGeometry = NULL;
			baseGeometry = dae.getDatabase()->idLookup(geometryReference, instance_controller->getDocument());

			//If no <geometry> exists that is referenced, skip this
			if(!baseGeometry) continue;
			#pragma endregion

			//Initialize a new MorphingMesh
			MorphingMesh* mesh = new MorphingMesh(Name);

			//Set this node to not be processed later
			process[i] = false;
			
			//Set the Base geometry
			mesh->Base = new Mesh(Name + "Base", processMatrix(nodes[i]->getDescendant("matrix")));
			mesh->Base->geometry = baseGeometry;

			#pragma endregion
			
			//Now find each <node> in the <visual_scenes> whos <instance_geometry> corresponds with a target
			for(unsigned int z = 0; z < nodes.getCount(); z++)
			{
				#pragma region //Get the geometry name and the <instance_geometry>

				//The name of the <geometry> node for this mesh
				string geometryName;

				//Get <instance_geometry>
				domInstance_geometry* instance_geometry = NULL;
				instance_geometry = (domInstance_geometry*)nodes[z]->getDescendant("instance_geometry");

				//If there is no <instance_geometry>, this isn't a static mesh and we will skip it.
				if(!instance_geometry) continue;

				//Get it's source url
				geometryName = instance_geometry->getAttribute("url").data();

				//Remove the "#"
				geometryName.erase(0, 1);

				#pragma endregion

				//Does this match any of the Targets?
				for(unsigned int x = 0; x < targets.size(); x++)
				{
					//Check
					if(targets[x] == geometryName)
					{
						#pragma region //Get its <geometry> node

						//Get the <geometry> node that is referenced by the <instance_geometry>
						daeElement* geometry = instance_geometry->getUrl().getElement();

						//If the referenced node was not found, skip this node
						if(!geometry) continue;

						#pragma endregion

						//Add to the Targets for this mesh
						mesh->Targets.push_back(new Mesh(nodes[z]->getAttribute("name"), processMatrix(nodes[z]->getDescendant("matrix"))));
						mesh->Targets.back()->geometry = geometry;

						//Set this node so it's not processed later
						process[z] = false;
					}
				}
			}

			Meshes->MorphingMeshes.push_back(mesh);
		}

		//For each <node>...
		for(unsigned int i = 0; i < nodes.getCount(); i++)
		{
			//Process this node?
			if(!process[i]) continue;

			//Get the name and the type, if they exist
			string Name = nodes[i]->getAttribute("name").data();
			string Type = nodes[i]->getAttribute("type").data();

			//Skip JOINT node's, only meshes
			if(Type == "JOINT") continue;
			
			//Get the <instance_controller> node that corresponds to this <node> if it exists
			domInstance_controller* instance_controller = NULL;
			instance_controller = (domInstance_controller*)nodes[i]->getDescendant("instance_controller");

			//If instance_controller exists, then this node is for a skinned mesh, else its a static mesh
			if(!instance_controller)
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

				//Now create a new Mesh, set it's <geometry> node and get it's World transform.
				Meshes->StaticMeshes.push_back(new Mesh(Name, processMatrix(nodes[i]->getDescendant("matrix"))));
				Meshes->StaticMeshes.back()->geometry = geometry;
			}
		}
	}

	//Process a string source
	vector<string> processStringSource(daeElement* source)
	{
		//Initialize Output array
		vector<string> Output = vector<string>();

		//Get raw string representation from the node
		string raw = source->getCharData();

		//Get the number of strings
		unsigned int count = atoi(source->getAttribute("count").data());

		//Set up stringstream
		stringstream stm(raw);

		//Extract each string
		for(unsigned int i = 0; i < count; i++)
		{
			string temp;

			stm >> temp;

			Output.push_back(temp);
		}

		//Return extracted strings
		return Output;
	}
	
	//Process a <geometry> node for each Static Mesh
	void processGeometries(MeshManager* Meshes)
	{
		//Foreach Static Mesh...
		for(unsigned int i = 0; i < Meshes->StaticMeshes.size(); i++)
		{
			//Get the <mesh> node
			daeElement* mesh = Meshes->StaticMeshes[i]->geometry->getDescendant("mesh");

			//Get the <source> nodes
			daeTArray<daeElementRef> sources = mesh->getChildren();

			//Get the <triangles> node (yes it will be in the sources array above if you wish to find it that way)
			daeElement* triangles = mesh->getDescendant("triangles");

			//Process each <source> child
			for(unsigned int z = 0; z < sources.getCount(); z++) processSource(Meshes->StaticMeshes[i], sources[z]);

			//Process the <triangles> child
			processTriangles(Meshes->StaticMeshes[i], triangles);
		}
			}

	//Process a <source> node for Static Mesh
	void processSource(Mesh* mesh, daeElement* source)
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

	
	//Process a <triangles> node for Static Mesh
	void processTriangles(Mesh* mesh, daeElement* triangles)
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
	
	//Read a <matrix> node, better to have this as a generalized function, will read into OpenGL style, conversion to DirectX later...
	D3DXMATRIX processMatrix(daeElement* matrix)
	{
		D3DXMATRIX out;
		string world = matrix->getCharData();
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
};
