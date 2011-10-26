#include "stdafx.h"
#include "ColladaStaticMesh.h"
#include <vector>

ColladaStaticMesh::ColladaStaticMesh()
{
	root = NULL;
	library_visual_scenes = NULL;
	library_animations = NULL;
}

ColladaStaticMesh::~ColladaStaticMesh()
{

}

StaticMesh* ColladaStaticMesh::Load( std::string filename )
{
	root = dae.open("..//asset//StaticExample.dae");

	if(!root)
	{
		//cout << "Document import failed. \n";
		return NULL;
	}

	//Get the library_visual_scenes
	library_visual_scenes = root->getDescendant("library_visual_scenes");
	//Check if there is a <library_visual_scenes>
	if(!library_visual_scenes)
	{
		//cout << "<library_visual_scenes> not found.\n";
		return NULL;
	}

	library_animations = root->getDescendant("library_animations");
	//Check if there is a <library_animations>
	if(!library_animations) 
		//cout << "<library_animations> not found.\n";

	//Process <library_visual_scenes>
	vector<int> Meshes;
	//Meshes.push_back( 1 );

	processVisualScenes( );

	dae.close(filename);

	return NULL;
}

void ColladaStaticMesh::processVisualScenes( )
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

			//Now create a new Mesh, set it's <geometry> node and get it's World transform.
			//Meshes.push_back(new Mesh(/*Name, processMatrix(nodes[i]->getDescendant("matrix"))*/));
			//Meshes.back()->geometry = geometry;
		}
	}
}
