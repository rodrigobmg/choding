#pragma once

#include <dae.h>
#include <dom.h>
#include <dom/domCOLLADA.h>

#include "..\..\EntitySystem\Component\Visual\StaticMesh.h"
#include <dae\daeElement.h>

#pragma comment( lib , "libcollada141dom21.lib" )

using namespace std;

class ColladaStaticMesh
{
private:
	//Dae file
	DAE dae;

	//Root node	
	domCOLLADA* root;
	
	//<library_visual_scenes> node
	daeElement* library_visual_scenes;

	//<library_animations> node
	daeElement* library_animations;

public:
	//Constructor
	ColladaStaticMesh();
	~ColladaStaticMesh();

	//Load all the meshes from a file
	StaticMesh* Load(std::string filename);

	void processVisualScenes();
};