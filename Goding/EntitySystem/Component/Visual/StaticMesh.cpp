#include "stdafx.h"
#include "StaticMesh.h"

StaticMesh::~StaticMesh(void)
{
	SAFE_RELEASE( m_indexBuffer );
	SAFE_RELEASE( m_vertexBuffer );
	SAFE_RELEASE( m_pEffect );
}
