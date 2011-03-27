#include "Precompiled.h"
#include "sunRenderNode.h"


ImplementRootRTTI( sunRenderNode );
ImplementBPool(sunVertexData);
ImplementBPool(sunIndexData);


sunRenderNode::sunRenderNode():
m_bVisible(true),
m_bSelected(false)
{

}



sunRenderNode::~sunRenderNode()
{
}

void sunRenderNode::BegineUpdate()
{
}

void sunRenderNode::Update()
{
}

void sunRenderNode::EndUpdate()
{
	
}

void sunRenderNode::GetRenderOperation( sunRenderOperation& op )
{
	op.m_spRenderNode = this;
	op.m_bUseIndexes = true;
	op.m_PrimType	= D3DPT_TRIANGLELIST;

	//op.m_pVertexData = 
	//
}


bool sunRenderNode::GetVisible()
{
	return m_bVisible;
}

void sunRenderNode::SetSelect(bool bFlag)
{
	m_bSelected = bFlag;
}

bool sunRenderNode::IsSelected()
{
	return m_bSelected;
}

void sunRenderNode::SetVisible(bool bFlag)
{
	m_bSelected = bFlag;
}


sunRenderOperation::sunRenderOperation()
{


}