#pragma once

#include <dae.h>
#include <dom.h>
#include <dom/domCOLLADA.h>

#include "..\..\EntitySystem\Component\Visual\StaticMesh.h"
#include <dae\daeElement.h>
#include <vector>

#ifdef _DEBUG
	#pragma comment( lib , "libcollada14dom22-d.lib" )
#else
	#pragma comment( lib , "libcollada14dom22.lib" )
#endif


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

	//<library_geometries> node
	daeElement* library_geometries;

	//<library_animations> node
	daeElement* library_animations;

	std::vector<StaticMesh*> mesh_container_t;	

public:
	//Constructor
	ColladaStaticMesh();
	~ColladaStaticMesh();

	//Load all the meshes from a file
	StaticMesh* Load(std::string filename);

	void processVisualScenes( std::vector<StaticMesh*>& Meshs );
	void processGeometries( std::vector<StaticMesh*>& Meshs );

	//Process a <source> node
	void processSource(StaticMesh* mesh, daeElement* source);
	//Process a <triangles> node
	void processTriangles(StaticMesh* mesh, daeElement* triangles);

	Matrix44 processMatrix(daeElement* matrix);
};