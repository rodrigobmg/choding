#include "GdsOctree.h"

GdsOctreeNode::GdsOctreeNode(int iNumFaces, D3DXVECTOR3& vMin, D3DXVECTOR3& vMax)
: m_minPos(vMin), m_maxPos(vMax), m_cenPos((m_minPos + m_maxPos)*0.5)
{
	// 자식 옥트리 노드
	_pChild[0] = _pChild[1] = _pChild[2] = _pChild[3] =
		_pChild[4] = _pChild[5] = _pChild[6] = _pChild[7] = NULL;
	_iNumChildren = 0;
	// 평면
	m_pFace = new GDSINDEX[iNumFaces];
	m_iCountIndexBuffer = iNumFaces;
}

void GdsOctreeNode::Empty()
{
	if(m_pFace){ delete [] m_pFace; m_pFace = NULL; }

	for(int i = 0; i < 8; ++i)
	{
		if(_pChild[i]){ delete _pChild[i]; _pChild[i] = NULL; }
	}
}

void GdsOctreeNode::Build()
{
	// 리프 노드는 바로 리턴
	if(m_iCountIndexBuffer <= ms_iLimitedCountOfFacePerNode) return;

	// 각 자식에 속하는 평면의 개수 셈
	D3DXVECTOR3 p0, p1, p2;
	int iFaceCount0 = 0, iFaceCount1 = 0, iFaceCount2 = 0, iFaceCount3 = 0;
	int iFaceCount4 = 0, iFaceCount5 = 0, iFaceCount6 = 0, iFaceCount7 = 0, iFaceCount8 = 0;
	GDSINDEX* pTempFace = NULL;

	for(int i = 0; i < m_iCountIndexBuffer; ++i)
	{
		p0 = D3DXVECTOR3(&_pVert[m_pFace[i]._0].p.x);
		p1 = D3DXVECTOR3(&_pVert[m_pFace[i]._1].p.x);
		p2 = D3DXVECTOR3(&_pVert[m_pFace[i]._2].p.x);

		if(
			// 아래 왼쪽 뒤
			(p0.x <= m_cenPos.x && p0.y <= m_cenPos.y && p0.z <= m_cenPos.z) &&
			(p1.x <= m_cenPos.x && p1.y <= m_cenPos.y && p1.z <= m_cenPos.z) &&
			(p2.x <= m_cenPos.x && p2.y <= m_cenPos.y && p2.z <= m_cenPos.z)
			) ++iFaceCount0;
		else if(
			// 아래 오른쪽 뒤
			(p0.x >= m_cenPos.x && p0.y <= m_cenPos.y && p0.z <= m_cenPos.z) &&
			(p1.x >= m_cenPos.x && p1.y <= m_cenPos.y && p1.z <= m_cenPos.z) &&
			(p2.x >= m_cenPos.x && p2.y <= m_cenPos.y && p2.z <= m_cenPos.z)
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
		++_iNumChildren;
		_pChild[0] = new GdsOctreeNode(iFaceCount0, m_minPos, m_cenPos);
	}
	if(iFaceCount1 > 0)
	{
		++_iNumChildren;
		_pChild[1] = new GdsOctreeNode(iFaceCount1, D3DXVECTOR3(m_cenPos.x, m_minPos.y, m_minPos.z), D3DXVECTOR3(m_maxPos.x, m_cenPos.y, m_cenPos.z));
	}
	if(iFaceCount2 > 0)
	{
		++_iNumChildren;
		_pChild[2] = new GdsOctreeNode(iFaceCount2, D3DXVECTOR3(m_minPos.x, m_minPos.y, m_cenPos.z), D3DXVECTOR3(m_cenPos.x, m_cenPos.y, m_maxPos.z));
	}
	if(iFaceCount3 > 0)
	{
		++_iNumChildren;
		_pChild[3] = new GdsOctreeNode(iFaceCount3, D3DXVECTOR3(m_cenPos.x, m_minPos.y, m_cenPos.z), D3DXVECTOR3(m_maxPos.x, m_cenPos.y, m_maxPos.z));
	}
	if(iFaceCount4 > 0)
	{
		++_iNumChildren;
		_pChild[4] = new GdsOctreeNode(iFaceCount4, D3DXVECTOR3(m_minPos.x, m_cenPos.y, m_minPos.z), D3DXVECTOR3(m_cenPos.x, m_maxPos.y, m_cenPos.z));
	}
	if(iFaceCount5 > 0)
	{
		++_iNumChildren;
		_pChild[5] = new GdsOctreeNode(iFaceCount5, D3DXVECTOR3(m_cenPos.x, m_cenPos.y, m_minPos.z), D3DXVECTOR3(m_maxPos.x, m_maxPos.y, m_cenPos.z));
	}
	if(iFaceCount6 > 0)
	{
		++_iNumChildren;
		_pChild[6] = new GdsOctreeNode(iFaceCount6, D3DXVECTOR3(m_minPos.z, m_cenPos.y, m_cenPos.z), D3DXVECTOR3(m_cenPos.x, m_maxPos.y, m_maxPos.z));
	}
	if(iFaceCount7 > 0)
	{
		++_iNumChildren;
		_pChild[7] = new GdsOctreeNode(iFaceCount7, m_cenPos, m_maxPos);
	}
	if(iFaceCount8 > 0)
	{
		pTempFace = new GDSINDEX[iFaceCount8];
		iFaceCount8 = 0;
	}

	// 자식에 속하는 평면을 모두 추가
	iFaceCount0 = 0, iFaceCount1 = 0, iFaceCount2 = 0, iFaceCount3 = 0;
	iFaceCount4 = 0, iFaceCount5 = 0, iFaceCount6 = 0, iFaceCount7 = 0, iFaceCount8 = 0;

	for(int i = 0; i < m_iCountIndexBuffer; ++i)
	{
		p0 = D3DXVECTOR3(&_pVert[m_pFace[i]._0].p.x);
		p1 = D3DXVECTOR3(&_pVert[m_pFace[i]._1].p.x);
		p2 = D3DXVECTOR3(&_pVert[m_pFace[i]._2].p.x);

		if(_pChild[0] &&
			(p0.x <= m_cenPos.x && p0.y <= m_cenPos.y && p0.z <= m_cenPos.z) &&
			(p1.x <= m_cenPos.x && p1.y <= m_cenPos.y && p1.z <= m_cenPos.z) &&
			(p2.x <= m_cenPos.x && p2.y <= m_cenPos.y && p2.z <= m_cenPos.z)
			) _pChild[0]->m_pFace[iFaceCount0++] = m_pFace[i];
		else if(_pChild[1] &&
			(p0.x >= m_cenPos.x && p0.y <= m_cenPos.y && p0.z <= m_cenPos.z) &&
			(p1.x >= m_cenPos.x && p1.y <= m_cenPos.y && p1.z <= m_cenPos.z) &&
			(p2.x >= m_cenPos.x && p2.y <= m_cenPos.y && p2.z <= m_cenPos.z)
			) _pChild[1]->m_pFace[iFaceCount1++] = m_pFace[i];
		else if(_pChild[2] &&
			(p0.x <= m_cenPos.x && p0.y <= m_cenPos.y && p0.z >= m_cenPos.z) &&
			(p1.x <= m_cenPos.x && p1.y <= m_cenPos.y && p1.z >= m_cenPos.z) &&
			(p2.x <= m_cenPos.x && p2.y <= m_cenPos.y && p2.z >= m_cenPos.z)
			) _pChild[2]->m_pFace[iFaceCount2++] = m_pFace[i];
		else if(_pChild[3] &&
			(p0.x >= m_cenPos.x && p0.y <= m_cenPos.y && p0.z >= m_cenPos.z) &&
			(p1.x >= m_cenPos.x && p1.y <= m_cenPos.y && p1.z >= m_cenPos.z) &&
			(p2.x >= m_cenPos.x && p2.y <= m_cenPos.y && p2.z >= m_cenPos.z)
			) _pChild[3]->m_pFace[iFaceCount3++] = m_pFace[i];
		else if(_pChild[4] &&
			(p0.x <= m_cenPos.x && p0.y >= m_cenPos.y && p0.z <= m_cenPos.z) &&
			(p1.x <= m_cenPos.x && p1.y >= m_cenPos.y && p1.z <= m_cenPos.z) &&
			(p2.x <= m_cenPos.x && p2.y >= m_cenPos.y && p2.z <= m_cenPos.z)
			) _pChild[4]->m_pFace[iFaceCount4++] = m_pFace[i];
		else if(_pChild[5] &&
			(p0.x >= m_cenPos.x && p0.y >= m_cenPos.y && p0.z <= m_cenPos.z) &&
			(p1.x >= m_cenPos.x && p1.y >= m_cenPos.y && p1.z <= m_cenPos.z) &&
			(p2.x >= m_cenPos.x && p2.y >= m_cenPos.y && p2.z <= m_cenPos.z)
			) _pChild[5]->m_pFace[iFaceCount5++] = m_pFace[i];
		else if(_pChild[6] &&
			(p0.x <= m_cenPos.x && p0.y >= m_cenPos.y && p0.z >= m_cenPos.z) &&
			(p1.x <= m_cenPos.x && p1.y >= m_cenPos.y && p1.z >= m_cenPos.z) &&
			(p2.x <= m_cenPos.x && p2.y >= m_cenPos.y && p2.z >= m_cenPos.z)
			) _pChild[6]->m_pFace[iFaceCount6++] = m_pFace[i];
		else if(_pChild[7] &&
			(p0.x >= m_cenPos.x && p0.y >= m_cenPos.y && p0.z >= m_cenPos.z) &&
			(p1.x >= m_cenPos.x && p1.y >= m_cenPos.y && p1.z >= m_cenPos.z) &&
			(p2.x >= m_cenPos.x && p2.y >= m_cenPos.y && p2.z >= m_cenPos.z)
			) _pChild[7]->m_pFace[iFaceCount7++] = m_pFace[i];
		else{
			pTempFace[iFaceCount8] = m_pFace[i];
			++iFaceCount8;
		}
	}

	// 자식 노드 재귀 호출
	if(_pChild[0]) _pChild[0]->Build();
	if(_pChild[1]) _pChild[1]->Build();
	if(_pChild[2]) _pChild[2]->Build();
	if(_pChild[3]) _pChild[3]->Build();
	if(_pChild[4]) _pChild[4]->Build();
	if(_pChild[5]) _pChild[5]->Build();
	if(_pChild[6]) _pChild[6]->Build();
	if(_pChild[7]) _pChild[7]->Build();

	// 리프 노드가 아니므로 평면 제거
	if(m_pFace)
	{
		delete [] m_pFace;
		m_pFace = NULL;
		m_iCountIndexBuffer = 0;
	}
	if(pTempFace)
	{
		m_pFace = pTempFace;
		m_iCountIndexBuffer = iFaceCount8;
	}
}

void GdsOctreeNode::Draw()
{
	// 시야 절두체 안에 포함되지(교차되지) 않는 노드는 그냥 리턴

	// 현재 노드에 속한 폴리곤이 있으면 드로잉
	for(int i = 0; i < m_iCountIndexBuffer; ++i)
	{
		// 드로잉 코드
	}

	if(_iNumChildren > 0)
	{
		// 자식 드로잉
		if(_pChild[0]) _pChild[0]->Draw();
		if(_pChild[1]) _pChild[1]->Draw();
		if(_pChild[2]) _pChild[2]->Draw();
		if(_pChild[3]) _pChild[3]->Draw();
		if(_pChild[4]) _pChild[4]->Draw();
		if(_pChild[5]) _pChild[5]->Draw();
		if(_pChild[6]) _pChild[6]->Draw();
		if(_pChild[7]) _pChild[7]->Draw();
	}
}