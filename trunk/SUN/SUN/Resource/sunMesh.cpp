#include "Precompiled.h"
#include "sunMesh.h"



ImplementBPool(sunMesh);

sunMesh::sunMesh()
{
	m_pVertexData = NULL;
	m_pIndexData  = NULL;

}

sunMesh::~sunMesh()
{
	Safe_Delete(m_pIndexData);
	Safe_Delete(m_pIndexData);
}


bool sunMesh::Load()
{

	return true;
}

int sunMesh::CreateVIB()
{


	return true;
}


sunVertexData* sunMesh::GetVertexData()
{
	return m_pVertexData;
}

sunIndexData* sunMesh::GetindexData()
{
	return 	m_pIndexData;
}
