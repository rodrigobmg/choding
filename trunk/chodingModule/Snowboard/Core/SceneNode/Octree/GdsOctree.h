#ifndef _GDS_CORE_NODE_OCTREE_H_
#define _GDS_CORE_NODE_OCTREE_H_

#include "../../Base/GdsObject.h"
#include "../../Renderer/GdsRendererDX9.h"

// ��Ʈ�� ���
class GdsOctreeNode
{
public:
	
	GdsOctreeNode(int iNumFaces, D3DXVECTOR3& vMin, D3DXVECTOR3& vMax);
	virtual ~GdsOctreeNode(){ Empty(); }

	void    Empty();
	void    Build();
	void    Draw();


private:
	static int			ms_iLimitedCountOfFacePerNode;      // ���� �ּ� ��� ��
	static GDSVERTEX*   _pVert;         // ���� �迭

	// �ڽ� ��Ʈ�� ���
	GdsOctreeNode*    _pChild[8];     // �ڽ� ���
	int     _iNumChildren;      // �ڽ� ��� ��
	// ���
	GDSINDEX*      m_pFace;         // ��� �迭
	int        m_iCountIndexBuffer;     // ��� ��
	// �ٿ�� �ڽ�
	D3DXVECTOR3     m_minPos;          // �ּ���
	D3DXVECTOR3     m_maxPos;          // �ִ���
	D3DXVECTOR3     m_cenPos;       // �߽���
};

#endif