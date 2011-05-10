#ifndef _GDS_CORE_NODE_OCTREE_H_
#define _GDS_CORE_NODE_OCTREE_H_

#include "../../Base/GdsObject.h"
#include "../../Renderer/GdsRendererDX9.h"

// 옥트리 노드
class GdsOctree
{
	struct Node
	{

		Node*		m_pChild[8];
		int			m_iNumOfChild;
		GDSINDEX*	m_pFace;
		int			m_iCountOfFace;
		D3DXVECTOR3	m_minPos;
		D3DXVECTOR3	m_maxPos;
		D3DXVECTOR3	m_cenPos;

		Node(int iNumFaces, D3DXVECTOR3& vMin, D3DXVECTOR3& vMax)
			: m_iCountOfFace( iNumFaces ) , m_minPos(vMin) , m_maxPos(vMax) , m_cenPos((vMin+vMax)*0.5 )
			, m_iNumOfChild( 0 )
		{
			for (int i=0 ; i<8 ; i++)
				m_pChild[i] = NULL;
			
			m_pFace = new GDSINDEX[m_iCountOfFace];
		}

		~Node()
		{			
			Clear();
		}

		void Clear()
		{
			for (int i=0 ; i<8 ; i++)
				SAFE_DELETE( m_pChild[i] );

			SAFE_DELETE( m_pFace );
		}

	};

	void			build( Node* node );
public:	

	GdsOctree(int iNumFaces, D3DXVECTOR3& vMin, D3DXVECTOR3& vMax , GDSVERTEX* pV , GDSINDEX* pI );
	virtual ~GdsOctree(){ Empty(); }

	void   			Empty();
	void   			Build(){ build( m_pRootNode ); }
	//void   			Draw();

	void			SetLimitedFacePerNode( int iCount ){ ms_iLimitedCountOfFacePerNode = iCount; }

private:
	int				ms_iLimitedCountOfFacePerNode;      // 노드당 최소 평면 수
	GDSVERTEX*		m_pVert;							// 정점 배열

	Node*			m_pRootNode;

};

#endif