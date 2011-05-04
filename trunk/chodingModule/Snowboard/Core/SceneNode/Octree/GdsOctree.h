#ifndef _GDS_CORE_NODE_OCTREE_H_
#define _GDS_CORE_NODE_OCTREE_H_

#include "../../Base/GdsObject.h"
#include "../../Renderer/GdsRendererDX9.h"

// 옥트리 노드
class GdsOctreeNode
{
public:
	
	GdsOctreeNode(int iNumFaces, D3DXVECTOR3& vMin, D3DXVECTOR3& vMax);
	virtual ~GdsOctreeNode(){ Empty(); }

	void    Empty();
	void    Build();
	void    Draw();


private:
	static int			ms_iLimitedCountOfFacePerNode;      // 노드당 최소 평면 수
	static GDSVERTEX*   _pVert;         // 정점 배열

	// 자식 옥트리 노드
	GdsOctreeNode*    _pChild[8];     // 자식 노드
	int     _iNumChildren;      // 자식 노드 수
	// 평면
	GDSINDEX*      m_pFace;         // 평면 배열
	int        m_iCountIndexBuffer;     // 평면 수
	// 바운딩 박스
	D3DXVECTOR3     m_minPos;          // 최소점
	D3DXVECTOR3     m_maxPos;          // 최대점
	D3DXVECTOR3     m_cenPos;       // 중심점
};

#endif