#include "stdafx.h"
#include "ColladaStaticMesh.h"

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

	//Close the .dae file
	dae.close(filename);

	return NULL;
}
