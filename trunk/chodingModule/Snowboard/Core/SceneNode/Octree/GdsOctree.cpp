#include "GdsOctree.h"

GdsOctree::GdsOctree(int iNumFaces, D3DXVECTOR3& vMin, D3DXVECTOR3& vMax , GDSVERTEX* pV , GDSINDEX* pI )
: m_pRootNode(NULL) , m_pVert( pV )
{
	if ( m_pRootNode == NULL )
		m_pRootNode = new Node( iNumFaces , vMin , vMax );

	m_pRootNode->m_pFace	= pI;
	m_pRootNode->m_iNumOfChild = 0;
}

void GdsOctree::Empty()
{
	SAFE_DELETE( m_pRootNode );
}

void GdsOctree::build( Node* node )
{
	int m_iNumChildren = node->m_iNumOfChild;
	int m_iCountOfFace = node->m_iCountOfFace;
	
	Node* m_pChild[8];
	for (int i=0;i<8 ;i++)
		m_pChild[i] = node->m_pChild[i];

	// 리프 노드는 바로 리턴
	if( m_iCountOfFace <= ms_iLimitedCountOfFacePerNode) return;

	// 각 자식에 속하는 평면의 개수 셈
	D3DXVECTOR3 p0, p1, p2;
	int iFaceCount0 = 0, iFaceCount1 = 0, iFaceCount2 = 0, iFaceCount3 = 0;
	int iFaceCount4 = 0, iFaceCount5 = 0, iFaceCount6 = 0, iFaceCount7 = 0, iFaceCount8 = 0;
	GDSINDEX* pTempFace = NULL;

	D3DXVECTOR3 m_cenPos = node->m_cenPos;
	D3DXVECTOR3 m_minPos = node->m_minPos;
	D3DXVECTOR3 m_maxPos = node->m_maxPos;
	GDSINDEX* m_pFace	 = node->m_pFace;

	for(int i = 0; i < node->m_iCountOfFace; ++i)
	{
		p0 = D3DXVECTOR3(&m_pVert[m_pFace[i]._0].p.x);
		p1 = D3DXVECTOR3(&m_pVert[m_pFace[i]._1].p.x);
		p2 = D3DXVECTOR3(&m_pVert[m_pFace[i]._2].p.x);

		if(
			// 아래 왼쪽 뒤
			(p0.x <= m_cenPos.x && p0.y <= m_cenPos.y && p0.z <= m_cenPos.z) &&
			(p1.x <= m_cenPos.x && p1.y <= m_cenPos.y && p1.z <= m_cenPos.z) &&
			(p2.x <= m_cenPos.x && p2.y <= m_cenPos.y && p2.z <= m_cenPos.z)
			) ++iFaceCount0;
		else if(
			// 아래 오른쪽 뒤
			(p0.x >= node->m_cenPos.x && p0.y <= node->m_cenPos.y && p0.z <= m_cenPos.z) &&
			(p1.x >= node->m_cenPos.x && p1.y <= node->m_cenPos.y && p1.z <= m_cenPos.z) &&
			(p2.x >= node->m_cenPos.x && p2.y <= node->m_cenPos.y && p2.z <= m_cenPos.z)
			) ++iFaceCount1;
		else if(
			// 아래 왼쪽 앞
			(p0.x <= m_cenPos.x && p0.y <= m_cenPos.y && p0.z >= m_cenPos.z) &&
			(p1.x <= m_cenPos.x && p1.y <= m_cenPos.y && p1.z >= m_cenPos.z) &&
			(p2.x <= m_cenPos.x && p2.y <= m_cenPos.y && p2.z >= m_cenPos.z)
			) ++iFaceCount2;
		else if(
			// 아래 오른쪽 앞
			(p0.x >= m_cenPos.x && p0.y <= m_cenPos.y && p0.z >= m_cenPos.z) &&
			(p1.x >= m_cenPos.x && p1.y <= m_cenPos.y && p1.z >= m_cenPos.z) &&
			(p2.x >= m_cenPos.x && p2.y <= m_cenPos.y && p2.z >= m_cenPos.z)
			) ++iFaceCount3;
		else if(
			// 위 왼쪽 뒤
			(p0.x <= m_cenPos.x && p0.y >= m_cenPos.y && p0.z <= m_cenPos.z) &&
			(p1.x <= m_cenPos.x && p1.y >= m_cenPos.y && p1.z <= m_cenPos.z) &&
			(p2.x <= m_cenPos.x && p2.y >= m_cenPos.y && p2.z <= m_cenPos.z)
			) ++iFaceCount4;
		else if(
			// 위 오른쪽 뒤
			(p0.x >= m_cenPos.x && p0.y >= m_cenPos.y && p0.z <= m_cenPos.z) &&
			(p1.x >= m_cenPos.x && p1.y >= m_cenPos.y && p1.z <= m_cenPos.z) &&
			(p2.x >= m_cenPos.x && p2.y >= m_cenPos.y && p2.z <= m_cenPos.z)
			) ++iFaceCount5;
		else if(
			// 위 왼쪽 앞
			(p0.x <= m_cenPos.x && p0.y >= m_cenPos.y && p0.z >= m_cenPos.z) &&
			(p1.x <= m_cenPos.x && p1.y >= m_cenPos.y && p1.z >= m_cenPos.z) &&
			(p2.x <= m_cenPos.x && p2.y >= m_cenPos.y && p2.z >= m_cenPos.z)
			) ++iFaceCount6;
		else if(
			// 위 오른쪽 앞
			(p0.x >= m_cenPos.x && p0.y >= m_cenPos.y && p0.z >= m_cenPos.z) &&
			(p1.x >= m_cenPos.x && p1.y >= m_cenPos.y && p1.z >= m_cenPos.z) &&
			(p2.x >= m_cenPos.x && p2.y >= m_cenPos.y && p2.z >= m_cenPos.z)
			) ++iFaceCount7;
		else{
			// 여러 자식에 걸쳐있는 평면
			++iFaceCount8;
		}
	}

	// 평면을 가진 자식만 생성
	if(iFaceCount0 > 0)
	{
		++m_iNumChildren;
		node->m_pChild[0] = new Node(iFaceCount0, m_minPos, m_cenPos);
	}
	if(iFaceCount1 > 0)
	{
		++m_iNumChildren;
		m_pChild[1] = new Node(iFaceCount1, D3DXVECTOR3(m_cenPos.x, m_minPos.y, m_minPos.z), D3DXVECTOR3(m_maxPos.x, m_cenPos.y, m_cenPos.z));
	}
	if(iFaceCount2 > 0)
	{
		++m_iNumChildren;
		m_pChild[2] = new Node(iFaceCount2, D3DXVECTOR3(m_minPos.x, m_minPos.y, m_cenPos.z), D3DXVECTOR3(m_cenPos.x, m_cenPos.y, m_maxPos.z));
	}
	if(iFaceCount3 > 0)
	{
		++m_iNumChildren;
		m_pChild[3] = new Node(iFaceCount3, D3DXVECTOR3(m_cenPos.x, m_minPos.y, m_cenPos.z), D3DXVECTOR3(m_maxPos.x, m_cenPos.y, m_maxPos.z));
	}
	if(iFaceCount4 > 0)
	{
		++m_iNumChildren;
		m_pChild[4] = new Node(iFaceCount4, D3DXVECTOR3(m_minPos.x, m_cenPos.y, m_minPos.z), D3DXVECTOR3(m_cenPos.x, m_maxPos.y, m_cenPos.z));
	}
	if(iFaceCount5 > 0)
	{
		++m_iNumChildren;
		m_pChild[5] = new Node(iFaceCount5, D3DXVECTOR3(m_cenPos.x, m_cenPos.y, m_minPos.z), D3DXVECTOR3(m_maxPos.x, m_maxPos.y, m_cenPos.z));
	}
	if(iFaceCount6 > 0)
	{
		++m_iNumChildren;
		m_pChild[6] = new Node(iFaceCount6, D3DXVECTOR3(m_minPos.z, m_cenPos.y, m_cenPos.z), D3DXVECTOR3(m_cenPos.x, m_maxPos.y, m_maxPos.z));
	}
	if(iFaceCount7 > 0)
	{
		++m_iNumChildren;
		m_pChild[7] = new Node(iFaceCount7, m_cenPos, m_maxPos);
	}
	if(iFaceCount8 > 0)
	{
		pTempFace = new GDSINDEX[iFaceCount8];
		iFaceCount8 = 0;
	}

	// 자식에 속하는 평면을 모두 추가
	iFaceCount0 = 0, iFaceCount1 = 0, iFaceCount2 = 0, iFaceCount3 = 0;
	iFaceCount4 = 0, iFaceCount5 = 0, iFaceCount6 = 0, iFaceCount7 = 0, iFaceCount8 = 0;

	for(int i = 0; i < m_iCountOfFace; ++i)
	{
		p0 = D3DXVECTOR3(&m_pVert[m_pFace[i]._0].p.x);
		p1 = D3DXVECTOR3(&m_pVert[m_pFace[i]._1].p.x);
		p2 = D3DXVECTOR3(&m_pVert[m_pFace[i]._2].p.x);

		if(m_pChild[0] &&
			(p0.x <= m_cenPos.x && p0.y <= m_cenPos.y && p0.z <= m_cenPos.z) &&
			(p1.x <= m_cenPos.x && p1.y <= m_cenPos.y && p1.z <= m_cenPos.z) &&
			(p2.x <= m_cenPos.x && p2.y <= m_cenPos.y && p2.z <= m_cenPos.z)
			) 
			m_pChild[0]->m_pFace[iFaceCount0++] = m_pFace[i];
		else if(m_pChild[1] &&
			(p0.x >= m_cenPos.x && p0.y <= m_cenPos.y && p0.z <= m_cenPos.z) &&
			(p1.x >= m_cenPos.x && p1.y <= m_cenPos.y && p1.z <= m_cenPos.z) &&
			(p2.x >= m_cenPos.x && p2.y <= m_cenPos.y && p2.z <= m_cenPos.z)
			)
			m_pChild[1]->m_pFace[iFaceCount1++] = m_pFace[i];
		else if(m_pChild[2] &&
			(p0.x <= m_cenPos.x && p0.y <= m_cenPos.y && p0.z >= m_cenPos.z) &&
			(p1.x <= m_cenPos.x && p1.y <= m_cenPos.y && p1.z >= m_cenPos.z) &&
			(p2.x <= m_cenPos.x && p2.y <= m_cenPos.y && p2.z >= m_cenPos.z)
			)
			m_pChild[2]->m_pFace[iFaceCount2++] = m_pFace[i];
		else if(m_pChild[3] &&
			(p0.x >= m_cenPos.x && p0.y <= m_cenPos.y && p0.z >= m_cenPos.z) &&
			(p1.x >= m_cenPos.x && p1.y <= m_cenPos.y && p1.z >= m_cenPos.z) &&
			(p2.x >= m_cenPos.x && p2.y <= m_cenPos.y && p2.z >= m_cenPos.z)
			)
			m_pChild[3]->m_pFace[iFaceCount3++] = m_pFace[i];
		else if(m_pChild[4] &&
			(p0.x <= m_cenPos.x && p0.y >= m_cenPos.y && p0.z <= m_cenPos.z) &&
			(p1.x <= m_cenPos.x && p1.y >= m_cenPos.y && p1.z <= m_cenPos.z) &&
			(p2.x <= m_cenPos.x && p2.y >= m_cenPos.y && p2.z <= m_cenPos.z)
			)
			m_pChild[4]->m_pFace[iFaceCount4++] = m_pFace[i];
		else if(m_pChild[5] &&
			(p0.x >= m_cenPos.x && p0.y >= m_cenPos.y && p0.z <= m_cenPos.z) &&
			(p1.x >= m_cenPos.x && p1.y >= m_cenPos.y && p1.z <= m_cenPos.z) &&
			(p2.x >= m_cenPos.x && p2.y >= m_cenPos.y && p2.z <= m_cenPos.z)
			)
			m_pChild[5]->m_pFace[iFaceCount5++] = m_pFace[i];
		else if(m_pChild[6] &&
			(p0.x <= m_cenPos.x && p0.y >= m_cenPos.y && p0.z >= m_cenPos.z) &&
			(p1.x <= m_cenPos.x && p1.y >= m_cenPos.y && p1.z >= m_cenPos.z) &&
			(p2.x <= m_cenPos.x && p2.y >= m_cenPos.y && p2.z >= m_cenPos.z)
			)
			m_pChild[6]->m_pFace[iFaceCount6++] = m_pFace[i];
		else if(m_pChild[7] &&
			(p0.x >= m_cenPos.x && p0.y >= m_cenPos.y && p0.z >= m_cenPos.z) &&
			(p1.x >= m_cenPos.x && p1.y >= m_cenPos.y && p1.z >= m_cenPos.z) &&
			(p2.x >= m_cenPos.x && p2.y >= m_cenPos.y && p2.z >= m_cenPos.z)
			)
			m_pChild[7]->m_pFace[iFaceCount7++] = m_pFace[i];
		else
		{
			pTempFace[iFaceCount8] = m_pFace[i];
			++iFaceCount8;
		}
	}

	// 자식 노드 재귀 호출
	if(m_pChild[0]) build( m_pChild[0] );
	if(m_pChild[1]) build( m_pChild[1] );
	if(m_pChild[2]) build( m_pChild[2] );
	if(m_pChild[3]) build( m_pChild[3] );
	if(m_pChild[4]) build( m_pChild[4] );
	if(m_pChild[5]) build( m_pChild[5] );
	if(m_pChild[6]) build( m_pChild[6] );
	if(m_pChild[7]) build( m_pChild[7] );

// 	if(m_pChild[0]) m_pChild[0]->Build();
// 	if(m_pChild[1]) m_pChild[1]->Build();
// 	if(m_pChild[2]) m_pChild[2]->Build();
// 	if(m_pChild[3]) m_pChild[3]->Build();
// 	if(m_pChild[4]) m_pChild[4]->Build();
// 	if(m_pChild[5]) m_pChild[5]->Build();
// 	if(m_pChild[6]) m_pChild[6]->Build();
// 	if(m_pChild[7]) m_pChild[7]->Build();

	// 리프 노드가 아니므로 평면 제거
	if(m_pFace)
	{
		delete [] m_pFace;
		m_pFace = NULL;
		m_iCountOfFace = 0;
	}
	if(pTempFace)
	{
		m_pFace = pTempFace;
		m_iCountOfFace = iFaceCount8;
	}
}

/*

void GdsOctree::Draw()
{
	// 시야 절두체 안에 포함되지(교차되지) 않는 노드는 그냥 리턴

	// 현재 노드에 속한 폴리곤이 있으면 드로잉
	for(int i = 0; i < m_iCountOfFace; ++i)
	{
		// 드로잉 코드
	}

	if(m_iNumChildren > 0)
	{
		// 자식 드로잉
		if(m_pChild[0]) m_pChild[0]->Draw();
		if(m_pChild[1]) m_pChild[1]->Draw();
		if(m_pChild[2]) m_pChild[2]->Draw();
		if(m_pChild[3]) m_pChild[3]->Draw();
		if(m_pChild[4]) m_pChild[4]->Draw();
		if(m_pChild[5]) m_pChild[5]->Draw();
		if(m_pChild[6]) m_pChild[6]->Draw();
		if(m_pChild[7]) m_pChild[7]->Draw();
	}
}*/
