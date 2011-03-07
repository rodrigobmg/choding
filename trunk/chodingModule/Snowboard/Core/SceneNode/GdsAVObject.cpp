#include "GdsAVObject.h"

GdsAVObject::GdsAVObject():
m_pParentNode(NULL)
{
	SetName( OBJECT_AVOBJECT );
}

GdsAVObject::~GdsAVObject()
{
}


HRESULT GdsAVObject::vUpdate( float fElapsedtime )
{
}


void GdsNode::SetParent( GdsNode* pNode )
{
	m_pParentNode = pNode;
}

GdsNode*	GdsNode::GetParent()
{
	return m_pParentNode;
}
