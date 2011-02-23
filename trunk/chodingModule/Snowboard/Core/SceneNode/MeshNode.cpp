#include "MeshNode.h"

MeshNode::MeshNode()
{
	SetName( OBJECT_SCENENODE_MESH );
}

MeshNode::~MeshNode()
{
}


HRESULT MeshNode::vUpdate( float fElapsedtime )
{
	TCHAR szlog[256] = {0,};
	_stprintf_s( szlog , L"%s" , L"I`m MeshNode" );
	++fElapsedtime;
	//Do something
	return S_OK;
}
