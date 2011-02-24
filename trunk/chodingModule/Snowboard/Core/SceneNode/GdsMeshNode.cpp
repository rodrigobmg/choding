
#include "GdsMeshNode.h"

GdsMeshNode::GdsMeshNode()
{
	SetName( OBJECT_NODE_MESH );
}

GdsMeshNode::~GdsMeshNode()
{
}


HRESULT GdsMeshNode::vUpdate( float fElapsedtime )
{
	TCHAR szlog[256] = {0,};
	_stprintf_s( szlog , L"%s" , L"I`m GdsMeshNode" );
	++fElapsedtime;
	//Do something
	return S_OK;
}
