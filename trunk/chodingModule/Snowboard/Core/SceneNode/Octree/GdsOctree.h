#ifndef _GDS_CORE_NODE_OCTREE_H_
#define _GDS_CORE_NODE_OCTREE_H_

#include "../../Base/GdsObject.h"
#include "../../Renderer/GdsRendererDX9.h"

class OcNode
{
	D3DXVECTOR3			m_minPos;
	D3DXVECTOR3			m_maxPos;
	D3DXVECTOR3			m_cenPos;

	GDSINDEX*			m_pIndex;
	OcNode*				m_pChild;
	OcNode*				m_Parent;
	int					m_iLimitedIndexCountPerNode;

public:
	OcNode(){};
	~OcNode(){};

	bool				devide( int iCurDepth );		
	void				build( D3DXVECTOR3& minPos , D3DXVECTOR3& maxPos );
	
};

class GdsOctree : public GdsObject
{	
	OcNode*				m_pNode;	
	
	void				clear();

public:

	GdsOctree(){ m_pNode = NULL; clear(); }
	virtual ~GdsOctree(){ clear(); }

	OcNode*				GetNode(){ return m_pNode; }
	void				Build( GDSVERTEX* pVertexBuffer , int iSizeVertexBuffer , GDSINDEX* pIndexBuffer , int iSizeIndexBuffer );

};

typedef boost::shared_ptr< GdsOctree >	GdsOctreePtr;

void GdsOctree::Build( GDSVERTEX* pVertexBuffer , int iSizeVertexBuffer , GDSINDEX* pIndexBuffer , int iSizeIndexBuffer )
{
	m_pNode = new OcNode;

	D3DXVECTOR3 minPos( 0,0,0 );
	D3DXVECTOR3 maxPos( 0,0,0 );
	for ( int i=0; i< iSizeVertexBuffer; i++ )
	{
		if ( minPos.x > pVertexBuffer[i].p.x )
			minPos.x = pVertexBuffer[i].p.x;

		if ( minPos.y > pVertexBuffer[i].p.y )
			minPos.y = pVertexBuffer[i].p.y;

		if ( minPos.z > pVertexBuffer[i].p.z )
			minPos.z = pVertexBuffer[i].p.z;

		if ( maxPos.x < pVertexBuffer[i].p.x )
			maxPos.x = pVertexBuffer[i].p.x;

		if ( maxPos.y < pVertexBuffer[i].p.y )
			maxPos.y = pVertexBuffer[i].p.y;

		if ( maxPos.z < pVertexBuffer[i].p.z )
			maxPos.z = pVertexBuffer[i].p.z;
	}
	D3DXVECTOR3 cenPos = ( maxPos + minPos ) * 0.5;

	D3DXVECTOR3 p0;
	D3DXVECTOR3 p1;
	D3DXVECTOR3 p2;

	int iFaceCount0 = 0, iFaceCount1 = 0, iFaceCount2 = 0, iFaceCount3 = 0;
	int iFaceCount4 = 0, iFaceCount5 = 0, iFaceCount6 = 0, iFaceCount7 = 0, iFaceCount8 = 0;
	for (int i = 0 ; i < iSizeIndexBuffer ; i++ )
	{
		p0 = pVertexBuffer[pIndexBuffer[i]._0].p;
		p1 = pVertexBuffer[pIndexBuffer[i]._1].p;
		p2 = pVertexBuffer[pIndexBuffer[i]._2].p;
		// 아래 왼쪽 뒤
		if( (p0.x <= cenPos.x && p0.y <= cenPos.y && p0.z <= cenPos.z) &&
			(p1.x <= cenPos.x && p1.y <= cenPos.y && p1.z <= cenPos.z) &&
			(p2.x <= cenPos.x && p2.y <= cenPos.y && p2.z <= cenPos.z)
			) ++iFaceCount0;
		else if(
			// 아래 오른쪽 뒤
			(p0.x >= cenPos.x && p0.y <= cenPos.y && p0.z <= cenPos.z) &&
			(p1.x >= cenPos.x && p1.y <= cenPos.y && p1.z <= cenPos.z) &&
			(p2.x >= cenPos.x && p2.y <= cenPos.y && p2.z <= cenPos.z)
			) ++iFaceCount1;
		else if(
			// 아래 왼쪽 앞
			(p0.x <= cenPos.x && p0.y <= cenPos.y && p0.z >= cenPos.z) &&
			(p1.x <= cenPos.x && p1.y <= cenPos.y && p1.z >= cenPos.z) &&
			(p2.x <= cenPos.x && p2.y <= cenPos.y && p2.z >= cenPos.z)
			) ++iFaceCount2;
		else if(
			// 아래 오른쪽 앞
			(p0.x >= cenPos.x && p0.y <= cenPos.y && p0.z >= cenPos.z) &&
			(p1.x >= cenPos.x && p1.y <= cenPos.y && p1.z >= cenPos.z) &&
			(p2.x >= cenPos.x && p2.y <= cenPos.y && p2.z >= cenPos.z)
			) ++iFaceCount3;
		else if(
			// 위 왼쪽 뒤
			(p0.x <= cenPos.x && p0.y >= cenPos.y && p0.z <= cenPos.z) &&
			(p1.x <= cenPos.x && p1.y >= cenPos.y && p1.z <= cenPos.z) &&
			(p2.x <= cenPos.x && p2.y >= cenPos.y && p2.z <= cenPos.z)
			) ++iFaceCount4;
		else if(
			// 위 오른쪽 뒤
			(p0.x >= cenPos.x && p0.y >= cenPos.y && p0.z <= cenPos.z) &&
			(p1.x >= cenPos.x && p1.y >= cenPos.y && p1.z <= cenPos.z) &&
			(p2.x >= cenPos.x && p2.y >= cenPos.y && p2.z <= cenPos.z)
			) ++iFaceCount5;
		else if(
			// 위 왼쪽 앞
			(p0.x <= cenPos.x && p0.y >= cenPos.y && p0.z >= cenPos.z) &&
			(p1.x <= cenPos.x && p1.y >= cenPos.y && p1.z >= cenPos.z) &&
			(p2.x <= cenPos.x && p2.y >= cenPos.y && p2.z >= cenPos.z)
			) ++iFaceCount6;
		else if(
			// 위 오른쪽 앞
			(p0.x >= cenPos.x && p0.y >= cenPos.y && p0.z >= cenPos.z) &&
			(p1.x >= cenPos.x && p1.y >= cenPos.y && p1.z >= cenPos.z) &&
			(p2.x >= cenPos.x && p2.y >= cenPos.y && p2.z >= cenPos.z)
			) ++iFaceCount7;
		else{
			// 여러 자식에 걸쳐있는 평면
			++iFaceCount8;
		}
	}
}
/*

// 정점
struct cVert
{
	float   x, y, z;
};

// 평면
struct cFace
{
	uint    v0;             // 첫번째 정점 인덱스
	uint    v1;             // 두번째 정점 인덱스
	uint    v2;             // 세번째 정점 인덱스
};

// 옥트리 노드
class cOctreeNode
{
public:
	cOctreeNode(int iNumFaces, cVector& vMin, cVector& vMax);
	virtual ~cOctreeNode(){ Empty(); }

	void    Empty();
	void    Build();
	void    Draw();

private:
	BOOL    IsInFrustum();

private:
	static uint _iMinFacesPerNode;      // 노드당 최소 평면 수
	static cVert*   _pVert;         // 정점 배열
	static cFrustum*    _pFrustum;      // 시야 절두체

	// 자식 옥트리 노드
	cOctreeNode*    _pChild[8];     // 자식 노드
	int     _iNumChildren;      // 자식 노드 수
	// 평면
	cFace*      _pFace;         // 평면 배열
	uint        _iNumFaces;     // 평면 수
	// 바운딩 박스
	cVector     _vMin;          // 최소점
	cVector     _vMax;          // 최대점
	cVector     _vCenter;       // 중심점
};

cOctreeNode::cOctreeNode(int iNumFaces, cVector& vMin, cVector& vMax)
: _vMin(vMin), _vMax(vMax), _vCenter((_vMin + _vMax)*0.5)
{
	// 자식 옥트리 노드
	_pChild[0] = _pChild[1] = _pChild[2] = _pChild[3] =
		_pChild[4] = _pChild[5] = _pChild[6] = _pChild[7] = NULL;
	_iNumChildren = 0;
	// 평면
	_pFace = new cFace[iNumFaces];
	_iNumFaces = iNumFaces;
}

void cOctreeNode::Empty()
{
	if(_pFace){ delete [] _pFace; _pFace = NULL; }

	for(int i = 0; i < 8; ++i)
	{
		if(_pChild[i]){ delete _pChild[i]; _pChild[i] = NULL; }
	}
}

void cOctreeNode::Build()
{
	// 리프 노드는 바로 리턴
	if(_iNumFaces <= _iMinFacesPerNode) return;

	// 각 자식에 속하는 평면의 개수 셈
	cVector p0, p1, p2;
	uint iFaceCount0 = 0, iFaceCount1 = 0, iFaceCount2 = 0, iFaceCount3 = 0;
	uint iFaceCount4 = 0, iFaceCount5 = 0, iFaceCount6 = 0, iFaceCount7 = 0, iFaceCount8 = 0;
	cFace* pTempFace = NULL;

	for(uint i = 0; i < _iNumFaces; ++i)
	{
		p0 = cVector(&_pVert[_pFace[i].v0].x);
		p1 = cVector(&_pVert[_pFace[i].v1].x);
		p2 = cVector(&_pVert[_pFace[i].v2].x);

		if(
			// 아래 왼쪽 뒤
			(p0.x <= _vCenter.x && p0.y <= _vCenter.y && p0.z <= _vCenter.z) &&
			(p1.x <= _vCenter.x && p1.y <= _vCenter.y && p1.z <= _vCenter.z) &&
			(p2.x <= _vCenter.x && p2.y <= _vCenter.y && p2.z <= _vCenter.z)
			) ++iFaceCount0;
		else if(
			// 아래 오른쪽 뒤
			(p0.x >= _vCenter.x && p0.y <= _vCenter.y && p0.z <= _vCenter.z) &&
			(p1.x >= _vCenter.x && p1.y <= _vCenter.y && p1.z <= _vCenter.z) &&
			(p2.x >= _vCenter.x && p2.y <= _vCenter.y && p2.z <= _vCenter.z)
			) ++iFaceCount1;
		else if(
			// 아래 왼쪽 앞
			(p0.x <= _vCenter.x && p0.y <= _vCenter.y && p0.z >= _vCenter.z) &&
			(p1.x <= _vCenter.x && p1.y <= _vCenter.y && p1.z >= _vCenter.z) &&
			(p2.x <= _vCenter.x && p2.y <= _vCenter.y && p2.z >= _vCenter.z)
			) ++iFaceCount2;
		else if(
			// 아래 오른쪽 앞
			(p0.x >= _vCenter.x && p0.y <= _vCenter.y && p0.z >= _vCenter.z) &&
			(p1.x >= _vCenter.x && p1.y <= _vCenter.y && p1.z >= _vCenter.z) &&
			(p2.x >= _vCenter.x && p2.y <= _vCenter.y && p2.z >= _vCenter.z)
			) ++iFaceCount3;
		else if(
			// 위 왼쪽 뒤
			(p0.x <= _vCenter.x && p0.y >= _vCenter.y && p0.z <= _vCenter.z) &&
			(p1.x <= _vCenter.x && p1.y >= _vCenter.y && p1.z <= _vCenter.z) &&
			(p2.x <= _vCenter.x && p2.y >= _vCenter.y && p2.z <= _vCenter.z)
			) ++iFaceCount4;
		else if(
			// 위 오른쪽 뒤
			(p0.x >= _vCenter.x && p0.y >= _vCenter.y && p0.z <= _vCenter.z) &&
			(p1.x >= _vCenter.x && p1.y >= _vCenter.y && p1.z <= _vCenter.z) &&
			(p2.x >= _vCenter.x && p2.y >= _vCenter.y && p2.z <= _vCenter.z)
			) ++iFaceCount5;
		else if(
			// 위 왼쪽 앞
			(p0.x <= _vCenter.x && p0.y >= _vCenter.y && p0.z >= _vCenter.z) &&
			(p1.x <= _vCenter.x && p1.y >= _vCenter.y && p1.z >= _vCenter.z) &&
			(p2.x <= _vCenter.x && p2.y >= _vCenter.y && p2.z >= _vCenter.z)
			) ++iFaceCount6;
		else if(
			// 위 오른쪽 앞
			(p0.x >= _vCenter.x && p0.y >= _vCenter.y && p0.z >= _vCenter.z) &&
			(p1.x >= _vCenter.x && p1.y >= _vCenter.y && p1.z >= _vCenter.z) &&
			(p2.x >= _vCenter.x && p2.y >= _vCenter.y && p2.z >= _vCenter.z)
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
		_pChild[0] = new cOctreeNode(iFaceCount0, _vMin, _vCenter);
	}
	if(iFaceCount1 > 0)
	{
		++_iNumChildren;
		_pChild[1] = new cOctreeNode(iFaceCount1, cVector(_vCenter.x, _vMin.y, _vMin.z), cVector(_vMax.x, _vCenter.y, _vCenter.z));
	}
	if(iFaceCount2 > 0)
	{
		++_iNumChildren;
		_pChild[2] = new cOctreeNode(iFaceCount2, cVector(_vMin.x, _vMin.y, _vCenter.z), cVector(_vCenter.x, _vCenter.y, _vMax.z));
	}
	if(iFaceCount3 > 0)
	{
		++_iNumChildren;
		_pChild[3] = new cOctreeNode(iFaceCount3, cVector(_vCenter.x, _vMin.y, _vCenter.z), cVector(_vMax.x, _vCenter.y, _vMax.z));
	}
	if(iFaceCount4 > 0)
	{
		++_iNumChildren;
		_pChild[4] = new cOctreeNode(iFaceCount4, cVector(_vMin.x, _vCenter.y, _vMin.z), cVector(_vCenter.x, _vMax.y, _vCenter.z));
	}
	if(iFaceCount5 > 0)
	{
		++_iNumChildren;
		_pChild[5] = new cOctreeNode(iFaceCount5, cVector(_vCenter.x, _vCenter.y, _vMin.z), cVector(_vMax.x, _vMax.y, _vCenter.z));
	}
	if(iFaceCount6 > 0)
	{
		++_iNumChildren;
		_pChild[6] = new cOctreeNode(iFaceCount6, cVector(_vMin.z, _vCenter.y, _vCenter.z), cVector(_vCenter.x, _vMax.y, _vMax.z));
	}
	if(iFaceCount7 > 0)
	{
		++_iNumChildren;
		_pChild[7] = new cOctreeNode(iFaceCount7, _vCenter, _vMax);
	}
	if(iFaceCount8 > 0)
	{
		pTempFace = new cFace[iFaceCount8];
		iFaceCount8 = 0;
	}

	// 자식에 속하는 평면을 모두 추가
	iFaceCount0 = 0, iFaceCount1 = 0, iFaceCount2 = 0, iFaceCount3 = 0;
	iFaceCount4 = 0, iFaceCount5 = 0, iFaceCount6 = 0, iFaceCount7 = 0, iFaceCount8 = 0;

	for(i = 0; i < _iNumFaces; ++i)
	{
		p0 = cVector(&_pVert[_pFace[i].v0].x);
		p1 = cVector(&_pVert[_pFace[i].v1].x);
		p2 = cVector(&_pVert[_pFace[i].v2].x);

		if(_pChild[0] &&
			(p0.x <= _vCenter.x && p0.y <= _vCenter.y && p0.z <= _vCenter.z) &&
			(p1.x <= _vCenter.x && p1.y <= _vCenter.y && p1.z <= _vCenter.z) &&
			(p2.x <= _vCenter.x && p2.y <= _vCenter.y && p2.z <= _vCenter.z)
			) _pChild[0]->_pFace[iFaceCount0++] = _pFace[i];
		else if(_pChild[1] &&
			(p0.x >= _vCenter.x && p0.y <= _vCenter.y && p0.z <= _vCenter.z) &&
			(p1.x >= _vCenter.x && p1.y <= _vCenter.y && p1.z <= _vCenter.z) &&
			(p2.x >= _vCenter.x && p2.y <= _vCenter.y && p2.z <= _vCenter.z)
			) _pChild[1]->_pFace[iFaceCount1++] = _pFace[i];
		else if(_pChild[2] &&
			(p0.x <= _vCenter.x && p0.y <= _vCenter.y && p0.z >= _vCenter.z) &&
			(p1.x <= _vCenter.x && p1.y <= _vCenter.y && p1.z >= _vCenter.z) &&
			(p2.x <= _vCenter.x && p2.y <= _vCenter.y && p2.z >= _vCenter.z)
			) _pChild[2]->_pFace[iFaceCount2++] = _pFace[i];
		else if(_pChild[3] &&
			(p0.x >= _vCenter.x && p0.y <= _vCenter.y && p0.z >= _vCenter.z) &&
			(p1.x >= _vCenter.x && p1.y <= _vCenter.y && p1.z >= _vCenter.z) &&
			(p2.x >= _vCenter.x && p2.y <= _vCenter.y && p2.z >= _vCenter.z)
			) _pChild[3]->_pFace[iFaceCount3++] = _pFace[i];
		else if(_pChild[4] &&
			(p0.x <= _vCenter.x && p0.y >= _vCenter.y && p0.z <= _vCenter.z) &&
			(p1.x <= _vCenter.x && p1.y >= _vCenter.y && p1.z <= _vCenter.z) &&
			(p2.x <= _vCenter.x && p2.y >= _vCenter.y && p2.z <= _vCenter.z)
			) _pChild[4]->_pFace[iFaceCount4++] = _pFace[i];
		else if(_pChild[5] &&
			(p0.x >= _vCenter.x && p0.y >= _vCenter.y && p0.z <= _vCenter.z) &&
			(p1.x >= _vCenter.x && p1.y >= _vCenter.y && p1.z <= _vCenter.z) &&
			(p2.x >= _vCenter.x && p2.y >= _vCenter.y && p2.z <= _vCenter.z)
			) _pChild[5]->_pFace[iFaceCount5++] = _pFace[i];
		else if(_pChild[6] &&
			(p0.x <= _vCenter.x && p0.y >= _vCenter.y && p0.z >= _vCenter.z) &&
			(p1.x <= _vCenter.x && p1.y >= _vCenter.y && p1.z >= _vCenter.z) &&
			(p2.x <= _vCenter.x && p2.y >= _vCenter.y && p2.z >= _vCenter.z)
			) _pChild[6]->_pFace[iFaceCount6++] = _pFace[i];
		else if(_pChild[7] &&
			(p0.x >= _vCenter.x && p0.y >= _vCenter.y && p0.z >= _vCenter.z) &&
			(p1.x >= _vCenter.x && p1.y >= _vCenter.y && p1.z >= _vCenter.z) &&
			(p2.x >= _vCenter.x && p2.y >= _vCenter.y && p2.z >= _vCenter.z)
			) _pChild[7]->_pFace[iFaceCount7++] = _pFace[i];
		else{
			pTempFace[iFaceCount8] = _pFace[i];
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
	if(_pFace)
	{
		delete [] _pFace;
		_pFace = NULL;
		_iNumFaces = 0;
	}
	if(pTempFace)
	{
		_pFace = pTempFace;
		_iNumFaces = iFaceCount8;
	}
}

void cOctreeNode::Draw()
{
	// 시야 절두체 안에 포함되지(교차되지) 않는 노드는 그냥 리턴
	if(!IsInFrustum()) return;

	// 현재 노드에 속한 폴리곤이 있으면 드로잉
	for(uint i = 0; i < _iNumFaces; ++i)
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
*/


#endif