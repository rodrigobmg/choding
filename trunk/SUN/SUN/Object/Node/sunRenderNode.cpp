#include "Precompiled.h"
#include "sunRenderNode.h"


ImplementRootRTTI( sunRenderNode );
ImplementBPool(sunVertexData);
ImplementBPool(sunIndexData);


sunRenderNode::sunRenderNode():
m_bVisible(true),
m_bSelected(false),
m_spMesh(NULL),
m_spMaterial(NULL),
m_spTrack(NULL),
m_fAccumulationTime(0.0f)
{

}

sunRenderNode::~sunRenderNode()
{
}

void sunRenderNode::BegineUpdate()
{

	if(m_spTrack)
	{
		float fElapsedTime = g_pCoreManager->GetElapsedTime();

		m_fAccumulationTime += (fElapsedTime * 100.0f);
		m_spTrack->UpdateTrack(m_fAccumulationTime);
		m_matAni = m_spTrack->GetAniMatrix();

		if(m_fAccumulationTime > 100.0f )
			m_fAccumulationTime = 0.0f;
	}

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
	op.m_pMaterial	 = SmartPointerCast( sunMaterial, m_spMaterial);
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

void sunRenderNode::SetMaterial( sunMaterialPtr spMaterial )
{
	assert( NULL != spMaterial);

	m_spMaterial = spMaterial;

}

sunMaterialPtr sunRenderNode::GetMaterial()
{
	return m_spMaterial;
}


void sunRenderNode::SetTrack( sunTrackPtr spTrack )
{
	assert( NULL != spTrack);

	m_spTrack = spTrack;

}

sunTrackPtr sunRenderNode::GetTrack()
{
	return m_spTrack;
}


sunRenderOperation::sunRenderOperation():
m_PrimType(D3DPT_LINELIST),
m_dwPrimCount(0),
m_bUseIndexes(false),
m_pIndexData(NULL),
m_pVertexData(NULL),
m_bUseTexture(false),
m_pMaterial(NULL)
{

}

sunRenderOperation::~sunRenderOperation()
{

}