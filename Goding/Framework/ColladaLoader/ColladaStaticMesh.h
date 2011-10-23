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
	MeshManager* Load(string filename);

private:
	//Process a <library_visual_scenes> node
	void processVisualScenes(MeshManager* Meshes);

	//Process a string source
	vector<string> processStringSource(daeElement* source);
	
	//Process a <geometry> node for each Static Mesh
	void processGeometries(MeshManager* Meshes);

	//Process a <source> node for Static Mesh
	void processSource(Mesh* mesh, daeElement* source);

	
	//Process a <triangles> node for Static Mesh
	void processTriangles(Mesh* mesh, daeElement* triangles);
	
	//Read a <matrix> node, better to have this as a generalized function, will read into OpenGL style, conversion to DirectX later...
	D3DXMATRIX processMatrix(daeElement* matrix);
};
