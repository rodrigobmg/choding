#include "GdsChildMeshNode.h"

GdsChildMeshNode::GdsChildMeshNode()
{
	SetName( OBJECT_NODE_CHILDMESH );
}

GdsChildMeshNode::~GdsChildMeshNode()
{
}


HRESULT GdsChildMeshNode::vUpdate( float fElapsedtime )
{
	TCHAR szlog[256] = {0,};
	_stprintf_s( szlog , L"%s" , L"I`m GdsChildMeshNode" );
	++fElapsedtime;
	//Do something
	return S_OK;
}
