#include "GdsOctree.h"

GdsOctree::GdsOctree(int iNumFaces, D3DXVECTOR3& vMin, D3DXVECTOR3& vMax , GDSVERTEX* pV , GDSINDEX* pI )
: m_pRootNode(NULL) , m_pVert( pV )
{
	if ( m_pRootNode == NULL )
		m_pRootNode = new Node( iNumFaces , vMin , vMax );

	memcpy( m_pRootNode->m_pFace , pI , sizeof( GDSINDEX ) * iNumFaces );
	m_pRootNode->m_iNumOfChild = 0;
}

void GdsOctree::Empty()
{
	SAFE_DELETE( m_pRootNode );
}

void GdsOctree::build( Node* node )
{
	int m_iNumChildren = 0;
	int m_iCountOfFace = node->m_iCountOfFace;
	
	Node* m_pChild[8];
	for (int i=0;i<8 ;i++)
		m_pChild[i] = NULL;//node->m_pChild[i];

	// ���� ���� �ٷ� ����
	if( m_iCountOfFace <= ms_iLimitedCountOfFacePerNode)
		return;

	// �� �ڽĿ� ���ϴ� ����� ���� ��
	D3DXVECTOR3 p0, p1, p2;
	int iFaceCount0 = 0, iFaceCount1 = 0, iFaceCount2 = 0, iFaceCount3 = 0;
	int iFaceCount4 = 0, iFaceCount5 = 0, iFaceCount6 = 0, iFaceCount7 = 0, iFaceCount8 = 0;
	GDSINDEX* pTempFace = NULL;

	D3DXVECTOR3 m_cenPos = node->m_cenPos;
	D3DXVECTOR3 m_minPos = node->m_minPos;
	D3DXVECTOR3 m_maxPos = node->m_maxPos;
	//GDSINDEX* m_pFace	 = node->m_pFace;

	for(int i = 0; i < node->m_iCountOfFace; ++i)
	{
		p0 = D3DXVECTOR3(&m_pVert[node->m_pFace[i]._0].p.x);
		p1 = D3DXVECTOR3(&m_pVert[node->m_pFace[i]._1].p.x);
		p2 = D3DXVECTOR3(&m_pVert[node->m_pFace[i]._2].p.x);

		if(
			// �Ʒ� ���� ��
			(p0.x <= m_cenPos.x && p0.y <= m_cenPos.y && p0.z <= m_cenPos.z) &&
			(p1.x <= m_cenPos.x && p1.y <= m_cenPos.y && p1.z <= m_cenPos.z) &&
			(p2.x <= m_cenPos.x && p2.y <= m_cenPos.y && p2.z <= m_cenPos.z)
			) 
			++iFaceCount0;
		else if(
			// �Ʒ� ������ ��
			(p0.x >= m_cenPos.x && p0.y <= m_cenPos.y && p0.z <= m_cenPos.z) &&
			(p1.x >= m_cenPos.x && p1.y <= m_cenPos.y && p1.z <= m_cenPos.z) &&
			(p2.x >= m_cenPos.x && p2.y <= m_cenPos.y && p2.z <= m_cenPos.z)
			)
			++iFaceCount1;
		else if(
			// �Ʒ� ���� ��
			(p0.x <= m_cenPos.x && p0.y <= m_cenPos.y && p0.z >= m_cenPos.z) &&
			(p1.x <= m_cenPos.x && p1.y <= m_cenPos.y && p1.z >= m_cenPos.z) &&
			(p2.x <= m_cenPos.x && p2.y <= m_cenPos.y && p2.z >= m_cenPos.z)
			)
			++iFaceCount2;
		else if(
			// �Ʒ� ������ ��
			(p0.x >= m_cenPos.x && p0.y <= m_cenPos.y && p0.z >= m_cenPos.z) &&
			(p1.x >= m_cenPos.x && p1.y <= m_cenPos.y && p1.z >= m_cenPos.z) &&
			(p2.x >= m_cenPos.x && p2.y <= m_cenPos.y && p2.z >= m_cenPos.z)
			) 
			++iFaceCount3;
		else if(
			// �� ���� ��
			(p0.x <= m_cenPos.x && p0.y >= m_cenPos.y && p0.z <= m_cenPos.z) &&
			(p1.x <= m_cenPos.x && p1.y >= m_cenPos.y && p1.z <= m_cenPos.z) &&
			(p2.x <= m_cenPos.x && p2.y >= m_cenPos.y && p2.z <= m_cenPos.z)
			) 
			++iFaceCount4;
		else if(
			// �� ������ ��
			(p0.x >= m_cenPos.x && p0.y >= m_cenPos.y && p0.z <= m_cenPos.z) &&
			(p1.x >= m_cenPos.x && p1.y >= m_cenPos.y && p1.z <= m_cenPos.z) &&
			(p2.x >= m_cenPos.x && p2.y >= m_cenPos.y && p2.z <= m_cenPos.z)
			) 
			++iFaceCount5;
		else if(
			// �� ���� ��
			(p0.x <= m_cenPos.x && p0.y >= m_cenPos.y && p0.z >= m_cenPos.z) &&
			(p1.x <= m_cenPos.x && p1.y >= m_cenPos.y && p1.z >= m_cenPos.z) &&
			(p2.x <= m_cenPos.x && p2.y >= m_cenPos.y && p2.z >= m_cenPos.z)
			) 
			++iFaceCount6;
		else if(
			// �� ������ ��
			(p0.x >= m_cenPos.x && p0.y >= m_cenPos.y && p0.z >= m_cenPos.z) &&
			(p1.x >= m_cenPos.x && p1.y >= m_cenPos.y && p1.z >= m_cenPos.z) &&
			(p2.x >= m_cenPos.x && p2.y >= m_cenPos.y && p2.z >= m_cenPos.z)
			) 
			++iFaceCount7;
		else{
			// ���� �ڽĿ� �����ִ� ���
			++iFaceCount8;
		}
	}

	// ����� ���� �ڽĸ� ����
	if(iFaceCount0 > 0)
	{
		++m_iNumChildren;
		m_pChild[0] = new Node(iFaceCount0, m_minPos, m_cenPos);
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
	}

	// �ڽĿ� ���ϴ� ����� ��� �߰�
	iFaceCount0 = 0, iFaceCount1 = 0, iFaceCount2 = 0, iFaceCount3 = 0;
	iFaceCount4 = 0, iFaceCount5 = 0, iFaceCount6 = 0, iFaceCount7 = 0, iFaceCount8 = 0;

	for(int i = 0; i < m_iCountOfFace; ++i)
	{
		p0 = D3DXVECTOR3(&m_pVert[node->m_pFace[i]._0].p.x);
		p1 = D3DXVECTOR3(&m_pVert[node->m_pFace[i]._1].p.x);
		p2 = D3DXVECTOR3(&m_pVert[node->m_pFace[i]._2].p.x);

		if(m_pChild[0] &&
			(p0.x <= m_cenPos.x && p0.y <= m_cenPos.y && p0.z <= m_cenPos.z) &&
			(p1.x <= m_cenPos.x && p1.y <= m_cenPos.y && p1.z <= m_cenPos.z) &&
			(p2.x <= m_cenPos.x && p2.y <= m_cenPos.y && p2.z <= m_cenPos.z)
			) 
			m_pChild[0]->m_pFace[iFaceCount0++] = node->m_pFace[i];
		else if(m_pChild[1] &&
			(p0.x >= m_cenPos.x && p0.y <= m_cenPos.y && p0.z <= m_cenPos.z) &&
			(p1.x >= m_cenPos.x && p1.y <= m_cenPos.y && p1.z <= m_cenPos.z) &&
			(p2.x >= m_cenPos.x && p2.y <= m_cenPos.y && p2.z <= m_cenPos.z)
			)
			m_pChild[1]->m_pFace[iFaceCount1++] = node->m_pFace[i];
		else if(m_pChild[2] &&
			(p0.x <= m_cenPos.x && p0.y <= m_cenPos.y && p0.z >= m_cenPos.z) &&
			(p1.x <= m_cenPos.x && p1.y <= m_cenPos.y && p1.z >= m_cenPos.z) &&
			(p2.x <= m_cenPos.x && p2.y <= m_cenPos.y && p2.z >= m_cenPos.z)
			)
			m_pChild[2]->m_pFace[iFaceCount2++] = node->m_pFace[i];
		else if(m_pChild[3] &&
			(p0.x >= m_cenPos.x && p0.y <= m_cenPos.y && p0.z >= m_cenPos.z) &&
			(p1.x >= m_cenPos.x && p1.y <= m_cenPos.y && p1.z >= m_cenPos.z) &&
			(p2.x >= m_cenPos.x && p2.y <= m_cenPos.y && p2.z >= m_cenPos.z)
			)
			m_pChild[3]->m_pFace[iFaceCount3++] = node->m_pFace[i];
		else if(m_pChild[4] &&
			(p0.x <= m_cenPos.x && p0.y >= m_cenPos.y && p0.z <= m_cenPos.z) &&
			(p1.x <= m_cenPos.x && p1.y >= m_cenPos.y && p1.z <= m_cenPos.z) &&
			(p2.x <= m_cenPos.x && p2.y >= m_cenPos.y && p2.z <= m_cenPos.z)
			)
			m_pChild[4]->m_pFace[iFaceCount4++] = node->m_pFace[i];
		else if(m_pChild[5] &&
			(p0.x >= m_cenPos.x && p0.y >= m_cenPos.y && p0.z <= m_cenPos.z) &&
			(p1.x >= m_cenPos.x && p1.y >= m_cenPos.y && p1.z <= m_cenPos.z) &&
			(p2.x >= m_cenPos.x && p2.y >= m_cenPos.y && p2.z <= m_cenPos.z)
			)
			m_pChild[5]->m_pFace[iFaceCount5++] = node->m_pFace[i];
		else if(m_pChild[6] &&
			(p0.x <= m_cenPos.x && p0.y >= m_cenPos.y && p0.z >= m_cenPos.z) &&
			(p1.x <= m_cenPos.x && p1.y >= m_cenPos.y && p1.z >= m_cenPos.z) &&
			(p2.x <= m_cenPos.x && p2.y >= m_cenPos.y && p2.z >= m_cenPos.z)
			)
			m_pChild[6]->m_pFace[iFaceCount6++] = node->m_pFace[i];
		else if(m_pChild[7] &&
			(p0.x >= m_cenPos.x && p0.y >= m_cenPos.y && p0.z >= m_cenPos.z) &&
			(p1.x >= m_cenPos.x && p1.y >= m_cenPos.y && p1.z >= m_cenPos.z) &&
			(p2.x >= m_cenPos.x && p2.y >= m_cenPos.y && p2.z >= m_cenPos.z)
			)
			m_pChild[7]->m_pFace[iFaceCount7++] = node->m_pFace[i];
		else
		{
			pTempFace[iFaceCount8] = node->m_pFace[i];
			++iFaceCount8;
		}
	}

	for (int i=0;i<8 ;i++)
		node->m_pChild[i] = m_pChild[i];

	node->m_iNumOfChild = m_iNumChildren;
	// ���� ��尡 �ƴϹǷ� ��� ����
	if(node->m_pFace)
	{
		SAFE_DELETE( node->m_pFace );
		m_iCountOfFace = 0;
	}
	if(pTempFace)
	{
		node->m_pFace = pTempFace;
		m_iCountOfFace = iFaceCount8;
	}

	// �ڽ� ��� ��� ȣ��
	if(m_pChild[0]) build( m_pChild[0] );
	if(m_pChild[1]) build( m_pChild[1] );
	if(m_pChild[2]) build( m_pChild[2] );
	if(m_pChild[3]) build( m_pChild[3] );
	if(m_pChild[4]) build( m_pChild[4] );
	if(m_pChild[5]) build( m_pChild[5] );
	if(m_pChild[6]) build( m_pChild[6] );
	if(m_pChild[7]) build( m_pChild[7] );


}

/*

void GdsOctree::Draw()
{
	// �þ� ����ü �ȿ� ���Ե���(��������) �ʴ� ���� �׳� ����

	// ���� ��忡 ���� �������� ������ �����
	for(int i = 0; i < m_iCountOfFace; ++i)
	{
		// ����� �ڵ�
	}

	if(m_iNumChildren > 0)
	{
		// �ڽ� �����
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
