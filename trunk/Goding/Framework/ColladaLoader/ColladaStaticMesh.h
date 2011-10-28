#pragma once

#include <vector>

#include <dae.h>
#include <dom.h>
#include <dom/domCOLLADA.h>
#include <dae\daeElement.h>

#include "..\..\EntitySystem\EntitySystem.h"
#include "StaticMesh\Mesh.h"

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

	std::vector<Mesh*> mesh_container_t;	

public:
	//Constructor
	ColladaStaticMesh();
	~ColladaStaticMesh();

	//Load all the meshes from a file
	void Load(std::string filename , EntitySystem* pEntitySystem );

	void processVisualScenes( std::vector<Mesh*>& Meshs );
	void processGeometries( std::vector<Mesh*>& Meshs );

	//Process a <source> node
	void processSource(Mesh* mesh, daeElement* source);
	//Process a <triangles> node
	void processTriangles(Mesh* mesh, daeElement* triangles);

	Matrix44 processMatrix(daeElement* matrix);

	void MakeMeshToComponent( std::vector<Mesh*>& Meshs );
};