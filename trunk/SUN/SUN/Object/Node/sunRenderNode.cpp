#include "Precompiled.h"
#include "sunRenderNode.h"


ImplementRootRTTI( sunRenderNode );
ImplementBPool(sunVertexData);
ImplementBPool(sunIndexData);


sunRenderNode::sunRenderNode():
m_bVisible(true),
m_bSelected(false),
m_spMesh(NULL)
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
	//assert( NULL != m_spMesh);

	op.m_spRenderNode = this;
	op.m_bUseIndexes = true;
	op.m_PrimType	= D3DPT_TRIANGLELIST;

	if(NULL == m_spMesh)
		return ;

	op.m_pVertexData = m_spMesh->GetVertexData();
	op.m_pIndexData  = m_spMesh->GetindexData();
	op.m_dwPrimCount = m_spMesh->GetindexData()->m_iIndexCount;
	
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

void sunRenderNode::SetMesh( sunMeshPtr spMesh )
{
	assert( NULL != spMesh);

	m_spMesh = spMesh;

}

sunMeshPtr sunRenderNode::GetMesh()
{
	return m_spMesh;
}


sunRenderOperation::sunRenderOperation():
m_PrimType(D3DPT_LINELIST),
m_dwPrimCount(0),
m_bUseIndexes(false),
m_pIndexData(NULL),
m_pVertexData(NULL),
m_bUseTexture(false)
{

}

sunRenderOperation::~sunRenderOperation()
{

}