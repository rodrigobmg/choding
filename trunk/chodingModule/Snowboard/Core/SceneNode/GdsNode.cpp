#include "GdsNode.h"
#include "Camera\GdsCameraManagerDX9.h"
#include "Renderer\GdsRendererDX9.h"

//ImplementBoostPool( GdsNode )
//boost::pool<> GdsNode::bpool( sizeof( GdsNode ) );

GdsNode::GdsNode():
m_bBillboard( false ),
m_vTranslate(0.0f, 0.0f, 0.0f),
m_vWorldTranslate(0.0f, 0.0f, 0.0f),
m_vScale( 1.0f, 1.0f, 1.0f)
, m_bUseOctree( false )
, m_pOctreeRootNode( NULL )
, m_iLimitedCountOfFacePerNode( 10 )
, m_bCull( false )
, m_bDrawOctreeBox( false )
, m_bUseLOD( false )
, m_pVBUseOnlyLOD(NULL)
, m_bUseQuadtree(false)
{
	SetName( OBJECT_NODE );
	m_ChildNode.clear();
	m_pParentNode = NULL;
// 	m_matWorld.MakeIdentity();
// 	m_matLocal.MakeIdentity();
	D3DXMatrixIdentity( &m_matWorld );
	D3DXMatrixIdentity( &m_matLocal );
	D3DXQuaternionIdentity(&m_qWorldRotate);
	D3DXQuaternionIdentity(&m_qRotate);
}

GdsNode::~GdsNode()
{
	vClear();	
}

void GdsNode::vClear()
{
	RemoveAllChild();	
	m_list_RenderObject.clear();
	ReleaseOctree();
	SAFE_DELETE( m_pVBUseOnlyLOD );
}


void GdsNode::GenOctreeFaceIndex()
{
	if ( !m_bUseOctree )
		return;

	RENDER_OBJECT_CONTAINER::iterator it = m_list_RenderObject.begin();
	for ( ; it != m_list_RenderObject.end() ; ++it )
	{
		LPDWORD pIB;
		GdsRenderObjectPtr rendertoken = it->first;
		LPDIRECT3DINDEXBUFFER9 pI = rendertoken->GetIndexBuffer();
		if( SUCCEEDED( pI->Lock( 0 , 0 , (void**)&pIB , 0 ) ) )
		{
			int iMaxCountIndex = genTriIndex( m_pOctreeRootNode , pIB , 0 );
			rendertoken->SetIndexMaxCount( iMaxCountIndex );
		}		
		pI->Unlock();
	}
}

int GdsNode::genTriIndex( Node* node , LPVOID pIB , int iCurIndexCount )
{

	if ( node->m_iCountOfFace > 0 )
	{
		if ( CAMMGR.GetCurCam()->GetFrustum().SphereIsInFrustum( node->m_cenPos , node->m_fRadius ) == false )
			return iCurIndexCount;

		if ( m_bUseLOD )
		{
			D3DXVECTOR3 vecdist = node->m_cenPos - m_vecCamPosUseOnlyLOD;
			float dist = D3DXVec3Length( &vecdist );

			for ( int i = 0 ; i < node->m_iCountOfFace ; i++ )
			{
				if ( i % 4 == 0 )
				{
					
				}
				GDSINDEX* p = ((GDSINDEX*)pIB) + iCurIndexCount;
				p->_0 = node->m_pFace[i]._0;
				p->_1 = node->m_pFace[i]._1;
				p->_2 = node->m_pFace[i]._2;			
				*p++;
				iCurIndexCount++;
			}		
		}
		else
		{
			for ( int i = 0 ; i < node->m_iCountOfFace ; i++ )
			{
				GDSINDEX* p = ((GDSINDEX*)pIB) + iCurIndexCount;
				p->_0 = node->m_pFace[i]._0;
				p->_1 = node->m_pFace[i]._1;
				p->_2 = node->m_pFace[i]._2;			
				*p++;
				iCurIndexCount++;
			}		
		}		

		if ( m_bDrawOctreeBox )
		{
			octreeDrawBox( node->m_minPos , node->m_maxPos );
		}

	}	

	if(node->m_pChild[0]) iCurIndexCount = genTriIndex( node->m_pChild[0] , pIB , iCurIndexCount );
	if(node->m_pChild[1]) iCurIndexCount = genTriIndex( node->m_pChild[1] , pIB , iCurIndexCount );
	if(node->m_pChild[2]) iCurIndexCount = genTriIndex( node->m_pChild[2] , pIB , iCurIndexCount );
	if(node->m_pChild[3]) iCurIndexCount = genTriIndex( node->m_pChild[3] , pIB , iCurIndexCount );
	if(node->m_pChild[4]) iCurIndexCount = genTriIndex( node->m_pChild[4] , pIB , iCurIndexCount );
	if(node->m_pChild[5]) iCurIndexCount = genTriIndex( node->m_pChild[5] , pIB , iCurIndexCount );
	if(node->m_pChild[6]) iCurIndexCount = genTriIndex( node->m_pChild[6] , pIB , iCurIndexCount );
	if(node->m_pChild[7]) iCurIndexCount = genTriIndex( node->m_pChild[7] , pIB , iCurIndexCount );

	return iCurIndexCount;
}

void GdsNode::CreateOctree()
{
	RENDER_OBJECT_CONTAINER::iterator it = m_list_RenderObject.begin();
	for ( ; it != m_list_RenderObject.end() ; ++it )
	{
		VOID* pVB;
		GdsRenderObjectPtr rendertoken = it->first;
		LPDIRECT3DVERTEXBUFFER9 vb = rendertoken->GetVertexBuffer();
		if (  SUCCEEDED( vb->Lock( 0 , rendertoken->GetVertexMaxCount() * sizeof( GDSVERTEX ) , (void**)&pVB , 0 ) ) )
		{
			D3DXVECTOR3 minPos( 0,0,0 );
			D3DXVECTOR3 maxPos( 0,0,0 );
			for ( int i=0 ; i < rendertoken->GetVertexMaxCount() ; i++ )
			{				
				GDSVERTEX* v = (GDSVERTEX*)pVB + i;

				if ( minPos.x > v->p.x )
					minPos.x = v->p.x;

				if ( minPos.y > v->p.y )
					minPos.y = v->p.y;

				if ( minPos.z > v->p.z )
					minPos.z = v->p.z;

				if ( maxPos.x < v->p.x )
					maxPos.x = v->p.x;
				if ( maxPos.y < v->p.y )
					maxPos.y = v->p.y;
				if ( maxPos.z < v->p.z )
					maxPos.z = v->p.z;
			}

			m_pOctreeRootNode = NULL;
			m_pVert = (GDSVERTEX*)pVB;
			if ( m_pOctreeRootNode == NULL )
				m_pOctreeRootNode = new Node( rendertoken->GetIndexMaxCount() , minPos , maxPos );

			VOID* pI;
			LPDIRECT3DINDEXBUFFER9 pIB = rendertoken->GetIndexBuffer();
			pIB->Lock( 0 , rendertoken->GetIndexMaxCount() * sizeof( GDSINDEX ) , (void**)&pI , 0 );
			memcpy( m_pOctreeRootNode->m_pFace , pI , sizeof( GDSINDEX ) * rendertoken->GetIndexMaxCount() );
			pIB->Unlock();
			m_pOctreeRootNode->m_iNumOfChild = 0;	

			build( m_pOctreeRootNode );
			
		}		
		vb->Unlock();
	}

	m_bUseOctree = true;
	m_iCountOfOctreeNode = 0;
}

void GdsNode::build( Node* node )
{
	int iNumChildren = 0;
	int iCountOfFace = node->m_iCountOfFace;

	Node* pChild[8];
	for (int i=0;i<8 ;i++)
		pChild[i] = NULL;//node->pChild[i];

	// 리프 노드는 바로 리턴
	if( iCountOfFace <= m_iLimitedCountOfFacePerNode)
		return;

	// 각 자식에 속하는 평면의 개수 셈
	D3DXVECTOR3 p0, p1, p2;
	int iFaceCount0 = 0, iFaceCount1 = 0, iFaceCount2 = 0, iFaceCount3 = 0;
	int iFaceCount4 = 0, iFaceCount5 = 0, iFaceCount6 = 0, iFaceCount7 = 0, iFaceCount8 = 0;
	GDSINDEX* pTempFace = NULL;

	D3DXVECTOR3 m_cenPos = node->m_cenPos;
	D3DXVECTOR3 m_minPos = node->m_minPos;
	D3DXVECTOR3 m_maxPos = node->m_maxPos;
	//GDSINDEX* m_pFace	 = node->m_pFace;

	for(int i = 0; i < iCountOfFace ; ++i)
	{
		p0 = D3DXVECTOR3(&m_pVert[node->m_pFace[i]._0].p.x);
		p1 = D3DXVECTOR3(&m_pVert[node->m_pFace[i]._1].p.x);
		p2 = D3DXVECTOR3(&m_pVert[node->m_pFace[i]._2].p.x);

		if(
			// 아래 왼쪽 뒤
			(p0.x <= m_cenPos.x && p0.y <= m_cenPos.y && p0.z <= m_cenPos.z) &&
			(p1.x <= m_cenPos.x && p1.y <= m_cenPos.y && p1.z <= m_cenPos.z) &&
			(p2.x <= m_cenPos.x && p2.y <= m_cenPos.y && p2.z <= m_cenPos.z)
			) 
			++iFaceCount0;
		else if(
			// 아래 오른쪽 뒤
			(p0.x >= m_cenPos.x && p0.y <= m_cenPos.y && p0.z <= m_cenPos.z) &&
			(p1.x >= m_cenPos.x && p1.y <= m_cenPos.y && p1.z <= m_cenPos.z) &&
			(p2.x >= m_cenPos.x && p2.y <= m_cenPos.y && p2.z <= m_cenPos.z)
			)
			++iFaceCount1;
		else if(
			// 아래 왼쪽 앞
			(p0.x <= m_cenPos.x && p0.y <= m_cenPos.y && p0.z >= m_cenPos.z) &&
			(p1.x <= m_cenPos.x && p1.y <= m_cenPos.y && p1.z >= m_cenPos.z) &&
			(p2.x <= m_cenPos.x && p2.y <= m_cenPos.y && p2.z >= m_cenPos.z)
			)
			++iFaceCount2;
		else if(
			// 아래 오른쪽 앞
			(p0.x >= m_cenPos.x && p0.y <= m_cenPos.y && p0.z >= m_cenPos.z) &&
			(p1.x >= m_cenPos.x && p1.y <= m_cenPos.y && p1.z >= m_cenPos.z) &&
			(p2.x >= m_cenPos.x && p2.y <= m_cenPos.y && p2.z >= m_cenPos.z)
			) 
			++iFaceCount3;
		else if(
			// 위 왼쪽 뒤
			(p0.x <= m_cenPos.x && p0.y >= m_cenPos.y && p0.z <= m_cenPos.z) &&
			(p1.x <= m_cenPos.x && p1.y >= m_cenPos.y && p1.z <= m_cenPos.z) &&
			(p2.x <= m_cenPos.x && p2.y >= m_cenPos.y && p2.z <= m_cenPos.z)
			) 
			++iFaceCount4;
		else if(
			// 위 오른쪽 뒤
			(p0.x >= m_cenPos.x && p0.y >= m_cenPos.y && p0.z <= m_cenPos.z) &&
			(p1.x >= m_cenPos.x && p1.y >= m_cenPos.y && p1.z <= m_cenPos.z) &&
			(p2.x >= m_cenPos.x && p2.y >= m_cenPos.y && p2.z <= m_cenPos.z)
			) 
			++iFaceCount5;
		else if(
			// 위 왼쪽 앞
			(p0.x <= m_cenPos.x && p0.y >= m_cenPos.y && p0.z >= m_cenPos.z) &&
			(p1.x <= m_cenPos.x && p1.y >= m_cenPos.y && p1.z >= m_cenPos.z) &&
			(p2.x <= m_cenPos.x && p2.y >= m_cenPos.y && p2.z >= m_cenPos.z)
			) 
			++iFaceCount6;
		else if(
			// 위 오른쪽 앞
			(p0.x >= m_cenPos.x && p0.y >= m_cenPos.y && p0.z >= m_cenPos.z) &&
			(p1.x >= m_cenPos.x && p1.y >= m_cenPos.y && p1.z >= m_cenPos.z) &&
			(p2.x >= m_cenPos.x && p2.y >= m_cenPos.y && p2.z >= m_cenPos.z)
			) 
			++iFaceCount7;
		else{
			// 여러 자식에 걸쳐있는 평면
			++iFaceCount8;
		}
	}

	// 평면을 가진 자식만 생성
	if(iFaceCount0 > 0)
	{
		++iNumChildren;
		pChild[0] = new Node(iFaceCount0, m_minPos, m_cenPos);
	}
	if(iFaceCount1 > 0)
	{
		++iNumChildren;
		D3DXVECTOR3 minPos = D3DXVECTOR3(m_cenPos.x, m_minPos.y, m_minPos.z);
		D3DXVECTOR3 maxPos = D3DXVECTOR3(m_maxPos.x, m_cenPos.y, m_cenPos.z);
		pChild[1] = new Node(iFaceCount1, minPos , maxPos );
	}
	if(iFaceCount2 > 0)
	{
		++iNumChildren;
		D3DXVECTOR3 minPos = D3DXVECTOR3(m_minPos.x, m_minPos.y, m_cenPos.z);
		D3DXVECTOR3 maxPos = D3DXVECTOR3(m_cenPos.x, m_cenPos.y, m_maxPos.z);
		pChild[2] = new Node(iFaceCount2, minPos , maxPos );
	}
	if(iFaceCount3 > 0)
	{
		++iNumChildren;
		D3DXVECTOR3 minPos = D3DXVECTOR3(m_cenPos.x, m_minPos.y, m_cenPos.z);
		D3DXVECTOR3 maxPos = D3DXVECTOR3(m_maxPos.x, m_cenPos.y, m_maxPos.z);
		pChild[3] = new Node(iFaceCount3, minPos, maxPos);
	}
	if(iFaceCount4 > 0)
	{
		++iNumChildren;
		D3DXVECTOR3 minPos = D3DXVECTOR3(m_minPos.x, m_cenPos.y, m_minPos.z);
		D3DXVECTOR3 maxPos = D3DXVECTOR3(m_cenPos.x, m_maxPos.y, m_cenPos.z);
		pChild[4] = new Node(iFaceCount4, minPos, maxPos);
	}
	if(iFaceCount5 > 0)
	{
		++iNumChildren;
		D3DXVECTOR3 minPos = D3DXVECTOR3(m_cenPos.x, m_cenPos.y, m_minPos.z);
		D3DXVECTOR3 maxPos = D3DXVECTOR3(m_maxPos.x, m_maxPos.y, m_cenPos.z);
		pChild[5] = new Node(iFaceCount5, minPos, maxPos);
	}
	if(iFaceCount6 > 0)
	{
		++iNumChildren;
		D3DXVECTOR3 minPos = D3DXVECTOR3(m_minPos.z, m_cenPos.y, m_cenPos.z);
		D3DXVECTOR3 maxPos = D3DXVECTOR3(m_cenPos.x, m_maxPos.y, m_maxPos.z);
		pChild[6] = new Node(iFaceCount6, minPos, maxPos);
	}
	if(iFaceCount7 > 0)
	{
		++iNumChildren;
		pChild[7] = new Node(iFaceCount7, m_cenPos, m_maxPos);
	}
	if(iFaceCount8 > 0)
	{
		pTempFace = new GDSINDEX[iFaceCount8];
	}

	// 자식에 속하는 평면을 모두 추가
	iFaceCount0 = 0, iFaceCount1 = 0, iFaceCount2 = 0, iFaceCount3 = 0;
	iFaceCount4 = 0, iFaceCount5 = 0, iFaceCount6 = 0, iFaceCount7 = 0, iFaceCount8 = 0;

	for(int i = 0; i < iCountOfFace; ++i)
	{
		p0 = D3DXVECTOR3(&m_pVert[node->m_pFace[i]._0].p.x);
		p1 = D3DXVECTOR3(&m_pVert[node->m_pFace[i]._1].p.x);
		p2 = D3DXVECTOR3(&m_pVert[node->m_pFace[i]._2].p.x);

		if(pChild[0] &&
			(p0.x <= m_cenPos.x && p0.y <= m_cenPos.y && p0.z <= m_cenPos.z) &&
			(p1.x <= m_cenPos.x && p1.y <= m_cenPos.y && p1.z <= m_cenPos.z) &&
			(p2.x <= m_cenPos.x && p2.y <= m_cenPos.y && p2.z <= m_cenPos.z)
			) 
			pChild[0]->m_pFace[iFaceCount0++] = node->m_pFace[i];
		else if(pChild[1] &&
			(p0.x >= m_cenPos.x && p0.y <= m_cenPos.y && p0.z <= m_cenPos.z) &&
			(p1.x >= m_cenPos.x && p1.y <= m_cenPos.y && p1.z <= m_cenPos.z) &&
			(p2.x >= m_cenPos.x && p2.y <= m_cenPos.y && p2.z <= m_cenPos.z)
			)
			pChild[1]->m_pFace[iFaceCount1++] = node->m_pFace[i];
		else if(pChild[2] &&
			(p0.x <= m_cenPos.x && p0.y <= m_cenPos.y && p0.z >= m_cenPos.z) &&
			(p1.x <= m_cenPos.x && p1.y <= m_cenPos.y && p1.z >= m_cenPos.z) &&
			(p2.x <= m_cenPos.x && p2.y <= m_cenPos.y && p2.z >= m_cenPos.z)
			)
			pChild[2]->m_pFace[iFaceCount2++] = node->m_pFace[i];
		else if(pChild[3] &&
			(p0.x >= m_cenPos.x && p0.y <= m_cenPos.y && p0.z >= m_cenPos.z) &&
			(p1.x >= m_cenPos.x && p1.y <= m_cenPos.y && p1.z >= m_cenPos.z) &&
			(p2.x >= m_cenPos.x && p2.y <= m_cenPos.y && p2.z >= m_cenPos.z)
			)
			pChild[3]->m_pFace[iFaceCount3++] = node->m_pFace[i];
		else if(pChild[4] &&
			(p0.x <= m_cenPos.x && p0.y >= m_cenPos.y && p0.z <= m_cenPos.z) &&
			(p1.x <= m_cenPos.x && p1.y >= m_cenPos.y && p1.z <= m_cenPos.z) &&
			(p2.x <= m_cenPos.x && p2.y >= m_cenPos.y && p2.z <= m_cenPos.z)
			)
			pChild[4]->m_pFace[iFaceCount4++] = node->m_pFace[i];
		else if(pChild[5] &&
			(p0.x >= m_cenPos.x && p0.y >= m_cenPos.y && p0.z <= m_cenPos.z) &&
			(p1.x >= m_cenPos.x && p1.y >= m_cenPos.y && p1.z <= m_cenPos.z) &&
			(p2.x >= m_cenPos.x && p2.y >= m_cenPos.y && p2.z <= m_cenPos.z)
			)
			pChild[5]->m_pFace[iFaceCount5++] = node->m_pFace[i];
		else if(pChild[6] &&
			(p0.x <= m_cenPos.x && p0.y >= m_cenPos.y && p0.z >= m_cenPos.z) &&
			(p1.x <= m_cenPos.x && p1.y >= m_cenPos.y && p1.z >= m_cenPos.z) &&
			(p2.x <= m_cenPos.x && p2.y >= m_cenPos.y && p2.z >= m_cenPos.z)
			)
			pChild[6]->m_pFace[iFaceCount6++] = node->m_pFace[i];
		else if(pChild[7] &&
			(p0.x >= m_cenPos.x && p0.y >= m_cenPos.y && p0.z >= m_cenPos.z) &&
			(p1.x >= m_cenPos.x && p1.y >= m_cenPos.y && p1.z >= m_cenPos.z) &&
			(p2.x >= m_cenPos.x && p2.y >= m_cenPos.y && p2.z >= m_cenPos.z)
			)
			pChild[7]->m_pFace[iFaceCount7++] = node->m_pFace[i];
		else
		{
			pTempFace[iFaceCount8] = node->m_pFace[i];
			++iFaceCount8;
		}
	}

	for (int i=0;i<8 ;i++)
		node->m_pChild[i] = pChild[i];
	m_iCountOfOctreeNode += iNumChildren;
	node->m_iNumOfChild = iNumChildren;
	// 리프 노드가 아니므로 평면 제거
	if(node->m_pFace)
	{
		SAFE_DELETE( node->m_pFace );
		iCountOfFace = 0;
	}
	if(pTempFace)
	{
		node->m_pFace = pTempFace;
		iCountOfFace = iFaceCount8;

		//페이스에 대한 minPos, maxPos를 갱신한다.
		D3DXVECTOR3 p0 , p1 , p2;
		D3DXVECTOR3 minPos( 0,0,0 );
		D3DXVECTOR3 maxPos( 0,0,0 );
		for(int i = 0; i < iCountOfFace ; ++i)
		{
			p0 = D3DXVECTOR3(&m_pVert[node->m_pFace[i]._0].p.x);
			p1 = D3DXVECTOR3(&m_pVert[node->m_pFace[i]._1].p.x);
			p2 = D3DXVECTOR3(&m_pVert[node->m_pFace[i]._2].p.x);
			
			if ( minPos.x > p0.x )
				minPos.x = p0.x;
			if ( minPos.y > p0.y )
				minPos.y = p0.y;
			if ( minPos.z > p0.z )
				minPos.z = p0.z;
			if ( maxPos.x < p0.x )
				maxPos.x = p0.x;
			if ( maxPos.y < p0.y )
				maxPos.y = p0.y;
			if ( maxPos.z < p0.z )
				maxPos.z = p0.z;

			if ( minPos.x > p1.x )
				minPos.x = p1.x;
			if ( minPos.y > p1.y )
				minPos.y = p1.y;
			if ( minPos.z > p1.z )
				minPos.z = p1.z;
			if ( maxPos.x < p1.x )
				maxPos.x = p1.x;
			if ( maxPos.y < p1.y )
				maxPos.y = p1.y;
			if ( maxPos.z < p1.z )
				maxPos.z = p1.z;

			if ( minPos.x > p2.x )
				minPos.x = p2.x;
			if ( minPos.y > p2.y )
				minPos.y = p2.y;
			if ( minPos.z > p2.z )
				minPos.z = p2.z;
			if ( maxPos.x < p2.x )
				maxPos.x = p2.x;
			if ( maxPos.y < p2.y )
				maxPos.y = p2.y;
			if ( maxPos.z < p2.z )
				maxPos.z = p2.z;
		}

		node->m_minPos = minPos;
		node->m_maxPos = maxPos;
		node->m_cenPos = (minPos+maxPos)*0.5;

	}

	D3DXVECTOR3 dist( m_cenPos - m_minPos );
	node->m_fRadius = D3DXVec3Length( &dist );
	node->m_iCountOfFace = iCountOfFace;

	// 자식 노드 재귀 호출
	if(node->m_pChild[0]) build( node->m_pChild[0] );
	if(node->m_pChild[1]) build( node->m_pChild[1] );
	if(node->m_pChild[2]) build( node->m_pChild[2] );
	if(node->m_pChild[3]) build( node->m_pChild[3] );
	if(node->m_pChild[4]) build( node->m_pChild[4] );
	if(node->m_pChild[5]) build( node->m_pChild[5] );
	if(node->m_pChild[6]) build( node->m_pChild[6] );
	if(node->m_pChild[7]) build( node->m_pChild[7] );
}


void GdsNode::SetLocalFromWorldTransform( const D3DXMATRIX& matWorld )
{
	if(m_pParentNode)
	{
		D3DXMATRIX matParentWorldInv;
		D3DXMatrixInverse( &matParentWorldInv, NULL, &m_pParentNode->GetWorldMatrix());
		D3DXMATRIX matLocal = matParentWorldInv * matWorld;
		SetLocalMatrix( matLocal );
	}
	else
	{
		SetLocalMatrix( matWorld );
	}

}


GdsNodePtr GdsNode::GetObjectbyName( tstring& strname )
{
	if ( !m_ChildNode.empty() )
	{
		CHILDNODE_CONTAINER::iterator it = m_ChildNode.begin();
		CHILDNODE_CONTAINER::iterator it_end = m_ChildNode.end();
		for ( ; it != it_end ; ++it )
		{
			if ( (*it)->GetName() == strname )
				return *(it);
		}
	}
	
	return GdsNodePtr( (GdsNode*)NULL );
}

HRESULT GdsNode::RemoveAllChild()
{
	if ( !m_ChildNode.empty() )
	{
		CHILDNODE_CONTAINER::iterator it = m_ChildNode.begin();
		for( ; it != m_ChildNode.end() ;  )
		{
			(*it)->RemoveAllChild();
			m_ChildNode.erase( it++ );
		}
	}
	return TRUE;
}

void GdsNode::SetParent( GdsNode* pNode )
{
	m_pParentNode = pNode;
}

GdsNode*	GdsNode::GetParent()
{
	return m_pParentNode;
}

// 0부터 시작함
GdsNodePtr	GdsNode::GetAt( unsigned int index )
{
	if ( m_ChildNode.empty() )
		return GdsNodePtr( (GdsNode*)NULL );

	if ( m_ChildNode.size() < index )
		return GdsNodePtr( (GdsNode*)NULL );

 	CHILDNODE_CONTAINER::iterator it = m_ChildNode.begin();
 	for ( size_t t = 0 ; it != m_ChildNode.end() ; ++it )
 	{
		if ( t == index )
			return (*it);

 		++t;
 	}

	ASSERT( 0 && L"못찾는건 말이 안된다. 차일드 리스트의 마지막 자식노드를 찾는중에 그 마지막 노드가 루프도중에 지워진거다;; ");
	return GdsNodePtr( (GdsNode*)NULL );
}


HRESULT GdsNode::AttachChild( GdsNodePtr pNode )
{
	if ( pNode == NULL )
		return false;

	pNode->SetParent( this );	
	m_ChildNode.push_back( pNode );
	return true;
}

HRESULT GdsNode::DetachChild( GdsNodePtr pNode )
{
	if ( this == pNode.get() )
		return false;

	CHILDNODE_CONTAINER::iterator it = m_ChildNode.begin();
	for( ; it != m_ChildNode.end() ; ++it )
	{
		if ( (*it) == pNode )
		{
			m_ChildNode.erase( it );
			break;
		}
	}

	pNode->SetParent( NULL );

	return true;
}

HRESULT GdsNode::Update( float fElapsedtime )
{	
	if ( m_bCull )
		return TRUE;

	D3DXMATRIX matTrans, matScale, matRot;
	D3DXMatrixIdentity( &matTrans );
	D3DXMatrixIdentity( &matScale );
	D3DXMatrixIdentity( &matRot );

	D3DXMatrixTranslation(&matTrans, m_vTranslate.x, m_vTranslate.y, m_vTranslate.z);
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixRotationQuaternion(&matRot, &m_qRotate);

	m_matLocal = matTrans * matRot * matScale;

	if( GetParent() )
	{
		D3DXMATRIX parTM;
		parTM = GetParent()->GetWorldMatrix();
		//m_matWorld = m_matLocal * m_matAni * m_pParent->GetWorldMatrix();
		m_matWorld = m_matLocal * parTM;
	}
	else
	{
		m_matWorld = m_matLocal;
	}

	m_vWorldTranslate = D3DXVECTOR3(m_matWorld._41, m_matWorld._42, m_matWorld._43 ) ;
	D3DXQuaternionRotationMatrix(&m_qWorldRotate, &m_matWorld);

	// 뷰 판정
	if ( CAMMGR.GetCurCam()->GetFrustum().VertexIsInFrustum( m_vWorldTranslate ) )	
	{
		if ( m_bUseOctree )
		{
			GenOctreeFaceIndex();
		}

		if ( !m_list_RenderObject.empty() )
		{
			RENDER_OBJECT_CONTAINER::iterator it = m_list_RenderObject.begin();
			for ( ; it != m_list_RenderObject.end() ; ++it )
			{
				RENDERER.GetRenderFrame()->AttachRenderObject( it->first , it->second );
			}			
		}		
	}
	else
	{

	}

	vUpdate( fElapsedtime );


	if ( !m_ChildNode.empty() )
	{
		for( CHILDNODE_CONTAINER::iterator it = m_ChildNode.begin() ; it != m_ChildNode.end() ; ++it )
		{
			(*it)->Update(fElapsedtime);
		}
	}		

	return TRUE;
}


void GdsNode::vUpdate( float fElapsedtime )
{
	if ( m_bBillboard == true )		
	{
// 		int32_t cur_cam_index = CAMMGR.GetCurCam();
// 		GdsCameraNodePtr pCamera = CAMMGR.GetCamNode( cur_cam_index );
// 		GdsMatrix3 billboard;
// 		billboard.MakeIdentity();
// 		GdsMatrix3 camMat = pCamera->GetWorldTransform().m_Rotate;
// 		billboard.SetEntry( 0 , 0 , camMat.m_pEntry[0][0] );
// 		billboard.SetEntry( 0 , 2 , camMat.m_pEntry[0][2] );
// 		billboard.SetEntry( 2 , 0 , camMat.m_pEntry[2][0] );
// 		billboard.SetEntry( 2 , 2 , camMat.m_pEntry[2][2] );
// 		billboard = billboard.Inverse();
// 
// 		GetWorldRotate() = billboard;
	}	
}


void GdsNode::AddRenderObject( GdsRenderObjectPtr pRenderObject , int iRenderStateIndex )
{
	RENDERTOKEN rendertoken( pRenderObject , iRenderStateIndex );
	m_list_RenderObject.push_back( rendertoken );	
}

void GdsNode::SetDrawAxis( bool bShow )
{
	RENDER_OBJECT_CONTAINER::iterator it = m_list_RenderObject.begin();
	RENDER_OBJECT_CONTAINER::iterator it_end = m_list_RenderObject.end();
	for ( ; it != it_end ; ++it )
	{
		it->first->SetDrawAxis( true );
	}
}

void GdsNode::SetDrawBox( bool bShow )
{
	RENDER_OBJECT_CONTAINER::iterator it = m_list_RenderObject.begin();
	RENDER_OBJECT_CONTAINER::iterator it_end = m_list_RenderObject.end();
	for ( ; it != it_end ; ++it )
	{
		it->first->SetDrawBox( true );
	}
}

void GdsNode::octreeDrawBox( D3DXVECTOR3& minPos , D3DXVECTOR3& maxPos )
{
	D3DXMATRIXA16 matWorld;
	D3DXMATRIXA16 matView;
	D3DXMATRIXA16 matProj;
	RENDERER.GetDevice()->GetTransform( D3DTS_WORLD , &matWorld );
	RENDERER.GetDevice()->GetTransform( D3DTS_VIEW  , &matView );
	RENDERER.GetDevice()->GetTransform( D3DTS_PROJECTION , &matProj );
	D3DXMATRIXA16 mat = matWorld*matView*matProj;

	D3DXVECTOR3 lineLBN[2];
	lineLBN[0] = minPos;
	lineLBN[1].x = minPos.x; lineLBN[1].y = maxPos.y; lineLBN[1].z = minPos.z;

	D3DXVECTOR3 lineLTN[2];
	lineLTN[0] = lineLBN[1];
	lineLTN[1].x = minPos.x; lineLTN[1].y = maxPos.y; lineLTN[1].z = maxPos.z;

	D3DXVECTOR3 lineLBF[2];
	lineLBF[0] = lineLTN[1];
	lineLBF[1].x = minPos.x; lineLBF[1].y = minPos.y; lineLBF[1].z = maxPos.z;

	D3DXVECTOR3 lineLTF[2];
	lineLTF[0] = lineLBF[1];
	lineLTF[1] = minPos;

	D3DXVECTOR3 lineRBN[2];
	lineRBN[0].x = maxPos.x; lineRBN[0].y = minPos.y; lineRBN[0].z = minPos.z;
	lineRBN[1].x = maxPos.x; lineRBN[1].y = maxPos.y; lineRBN[1].z = minPos.z;

	D3DXVECTOR3 lineRTN[2];
	lineRTN[0] = lineRBN[1];
	lineRTN[1].x = maxPos.x; lineRTN[1].y = maxPos.y; lineRTN[1].z = maxPos.z;

	D3DXVECTOR3 lineRBF[2];
	lineRBF[0] = lineRTN[1];
	lineRBF[1].x = maxPos.x; lineRBF[1].y = minPos.y; lineRBF[1].z = maxPos.z;

	D3DXVECTOR3 lineRTF[2];
	lineRTF[0] = lineRBF[1];
	lineRTF[1] = lineRBN[0];

	D3DXVECTOR3 lineBN[2];
	lineBN[0] = minPos;
	lineBN[1].x = maxPos.x; lineBN[1].y = minPos.y; lineBN[1].z = minPos.z;

	D3DXVECTOR3 lineTN[2];
	lineTN[0].x = minPos.x; lineTN[0].y = maxPos.y; lineTN[0].z = minPos.z;
	lineTN[1].x = maxPos.x; lineTN[1].y = maxPos.y; lineTN[1].z = minPos.z;

	D3DXVECTOR3 lineBF[2];
	lineBF[0].x = minPos.x; lineBF[0].y = minPos.y; lineBF[0].z = maxPos.z;
	lineBF[1].x = maxPos.x; lineBF[1].y = minPos.y; lineBF[1].z = maxPos.z;

	D3DXVECTOR3 lineTF[2];
	lineBF[0].x = minPos.x; lineBF[0].y = maxPos.y; lineBF[0].z = maxPos.z;
	lineBF[1] = maxPos;

	ID3DXLine* Line;
	D3DXCreateLine( RENDERER.GetDevice() , &Line );
	Line->SetWidth( 1 );
	Line->SetAntialias( true );
	Line->Begin();
	Line->DrawTransform( lineLBN , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineLTN , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineLBF , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineLTF , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineRBN , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineRTN , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineRBF , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineRTF , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineBN , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineTN , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineBF , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineTF , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));

	Line->End();
	Line->Release();	
}

void GdsNode::SetUseLOD( bool flag )
{
	m_bUseLOD = flag;
	if ( flag )
	{
		m_vecCamPosUseOnlyLOD = CAMMGR.GetCurCam()->GetEye();
		SAFE_DELETE( m_pVBUseOnlyLOD );
		GdsRenderObjectPtr rendertoken = GetRenderObject( 0 );
		VOID* pVB;
		LPDIRECT3DVERTEXBUFFER9 vb = rendertoken->GetVertexBuffer();
		if (  SUCCEEDED( vb->Lock( 0 , rendertoken->GetVertexMaxCount() * sizeof( GDSVERTEX ) , (void**)&pVB , 0 ) ) )
		{
			m_pVBUseOnlyLOD = new GDSVERTEX[ rendertoken->GetVertexMaxCount() ];
			memcpy( m_pVBUseOnlyLOD , pVB , sizeof( GDSVERTEX )* rendertoken->GetVertexMaxCount() );
		}
		vb->Unlock();
	}
}

/*
#pragma	  once
#include "SmartIndexBuffer.h"

class SectorSLodPatch
{
public:
	enum PatchDir
	{
		PATCHDIR_LEFT = 0,
		PATCHDIR_RIGHT,
		PATCHDIR_TOP,
		PATCHDIR_BOTTOM
	};

	static const int MaxLevel = 4;

private:
	struct OneLevel
	{
		Smart16BitIndexBuffer Normal[4];
		Smart16BitIndexBuffer ProtectionFromCrack[4];
	};

	static const int Width;
	static const int HalfWidth;

public:
	SectorSLodPatch();
	virtual ~SectorSLodPatch();

	void Draw(PatchDir dir, int TargetLevel, bool IsProtectCrack);

private:
	void CreateNormalIndice();
	void CreateProectionFromCrackIndice();
	inline void ConvertIndex(PatchDir dir, int origin_x, int origin_y, int& new_x, int& new_y);
	inline int ComputeRealIndex(int x, int y);
	inline void AddIndex(Smart16BitIndexBuffer& targetib, int n, int x, int y);
	void CheckErrorCCW(Smart16BitIndexBuffer& targetib, int start, int end, PatchDir dir);

private:
	int m_MaxLevel;
	OneLevel m_Indice[MaxLevel];
};

// 각 조각에 대해 아래와 같이 좌표를 변환해 줌
inline void SectorSLodPatch::ConvertIndex(PatchDir dir, int origin_x, int origin_y, int& new_x, int& new_y)
{
	switch(dir)
	{
	case PATCHDIR_LEFT:
		new_x = origin_x;
		new_y = origin_y;
		break;

	case PATCHDIR_RIGHT:
		new_x = -origin_x + Width;
		new_y = origin_y;
		break;

	case PATCHDIR_TOP:
		new_x = origin_y;
		new_y = origin_x;
		break;

	case PATCHDIR_BOTTOM:
		new_x = origin_y;
		new_y = -origin_x + Width;
		break;
	}
}

// 실제 index buffer에 기록할 좌표 계산
inline int SectorSLodPatch::ComputeRealIndex(int x, int y)
{
	return y*(Width+1)+x;
}

// index 추가
inline void SectorSLodPatch::AddIndex(Smart16BitIndexBuffer& targetib, int n, int x, int y)
{
	int newx, newy;
	ConvertIndex((PatchDir)n, x, y, newx, newy);
	targetib.AddIndex(ComputeRealIndex(newx, newy));
}















#include "stdafx.h"
#include "SectorSLodPatch.h"
#include "HeightMap.h"

const int SectorSLodPatch::Width = HeightMap::TableCellWidth-1;
const int SectorSLodPatch::HalfWidth = (HeightMap::TableCellWidth-1)/2;

SectorSLodPatch::SectorSLodPatch()
{
	CreateNormalIndice();
	CreateProectionFromCrackIndice();
}

SectorSLodPatch::~SectorSLodPatch()
{
}

// 렌더링....
void SectorSLodPatch::Draw(PatchDir dir, int TargetLevel, bool IsProtectCrack)
{
	if(TargetLevel >= MaxLevel)
		TargetLevel = MaxLevel-1;
	if(IsProtectCrack)
	{
		m_Indice[TargetLevel].ProtectionFromCrack[dir].SetIndices();
		m_Indice[TargetLevel].ProtectionFromCrack[dir].Draw();
	}
	else
	{
		m_Indice[TargetLevel].Normal[dir].SetIndices();
		m_Indice[TargetLevel].Normal[dir].Draw();
	}
}

// 일반 조각 만들기
void SectorSLodPatch::CreateNormalIndice()
{
	int curwidth = Width;

	// 각 레벨 만들기
	for(int m = 0 ; m < MaxLevel ; m++, curwidth /= 2)
	{
		int indexcount = 0;
		for(int n = 2 ; n <= curwidth ; n += 2)
			indexcount += n * 2 + 1;

		const int increase_gap = Width/curwidth;

		// 각 방향 만들기
		for(int n = 0 ; n < 4 ; n++)
		{
			Smart16BitIndexBuffer& targetib = m_Indice[m].Normal[n];
			targetib.CreateIndexArray(indexcount);
			targetib.CreateIndexBuffer();
			targetib.ResetDynamicIndexPtr();

			int startpntx = 0, startpnty = 0;
			int x = 0, y = 0;

			// 스트립 만들기
			while(startpntx != HalfWidth)
			{
				int startindex = targetib.GetDynamicIndexDataNumber();

				AddIndex(targetib, n , x, y);

				// 스트립 만들때 index 를 추가할 세가지 type
				while(x+y != Width)
				{
					if(y<HalfWidth)
					{
						x+=increase_gap;
						y+=increase_gap;
						AddIndex(targetib, n, x, y);

						x-=increase_gap;
						AddIndex(targetib, n, x, y);
					}
					else if(y==HalfWidth)
					{
						x+=increase_gap;
						AddIndex(targetib, n, x, y);

						x-=increase_gap;
						y+=increase_gap;
						AddIndex(targetib, n, x, y);
					}
					else
					{
						x+=increase_gap;
						AddIndex(targetib, n, x, y);

						x-=increase_gap;
						y+=increase_gap;
						AddIndex(targetib, n, x, y);
					}
				}

				int endindex = targetib.GetDynamicIndexDataNumber();
				targetib.Check(D3DPT_TRIANGLESTRIP);

				CheckErrorCCW(targetib, startindex, endindex, (PatchDir)n);

				startpntx += increase_gap;
				startpnty += increase_gap;
				x = startpntx;
				y = startpnty;
			}

			targetib.Upload();
			targetib.DestroyIndexArray();
		}
	}
}

// Clock-Wise인 경우 Counter-Clock-Wise 로 변환
void SectorSLodPatch::CheckErrorCCW(Smart16BitIndexBuffer& targetib, int start, int end, PatchDir dir)
{
	if(dir == PATCHDIR_LEFT || dir == PATCHDIR_BOTTOM)
	{
		// cull ccw 때문에 뒤집어야 함
		for(int k = 0; k < (end - start)/2 ; k++)
			swap(targetib[start + k], targetib[end - k - 1]);
	}
}

// 크랙방지용 조각 만들기
void SectorSLodPatch::CreateProectionFromCrackIndice()
{
	int curwidth = Width;

	// 각 레벨 만들기
	for(int m = 0 ; m < MaxLevel ; m++, curwidth /= 2)
	{
		int indexcount = 3*(curwidth/2) + 6*(curwidth/2-1);
		for(int n = 2 ; n < curwidth ; n += 2)
			indexcount += n * 2 + 1;

		const int increase_gap = Width/curwidth;

		// 각 방향 만들기
		for(int n = 0 ; n < 4 ; n++)
		{
			Smart16BitIndexBuffer& targetib = m_Indice[m].ProtectionFromCrack[n];
			targetib.CreateIndexArray(indexcount);
			targetib.CreateIndexBuffer();
			targetib.ResetDynamicIndexPtr();

			// crack 방지 triangle list
			int startindex = targetib.GetDynamicIndexDataNumber();

			for(int k=0;k<Width;k+=increase_gap*2)
			{
				AddIndex(targetib, n, 0, k);
				AddIndex(targetib, n, increase_gap, k+increase_gap);
				AddIndex(targetib, n, 0, k+increase_gap*2);
			}
			for(int k=increase_gap*2; k<=Width-increase_gap*2; k+=increase_gap*2)
			{
				AddIndex(targetib, n, increase_gap, k);
				AddIndex(targetib, n, 0, k);
				AddIndex(targetib, n, increase_gap, k-increase_gap);
				AddIndex(targetib, n, increase_gap, k);
				AddIndex(targetib, n, increase_gap, k+increase_gap);
				AddIndex(targetib, n, 0, k);
			}

			int endindex = targetib.GetDynamicIndexDataNumber();
			targetib.Check(D3DPT_TRIANGLELIST);

			CheckErrorCCW(targetib, startindex, endindex, (PatchDir)n);

			int startpntx = increase_gap, startpnty = increase_gap;
			int x = increase_gap;
			int y = increase_gap;

			// 스트립 만들기
			while(startpntx != HalfWidth)
			{
				startindex = targetib.GetDynamicIndexDataNumber();

				AddIndex(targetib, n, x, y);

				// 스트립 만들때 index 를 추가할 세가지 type
				while(x+y != Width)
				{
					if(y<HalfWidth)
					{
						x+=increase_gap;
						y+=increase_gap;
						AddIndex(targetib, n, x, y);

						x-=increase_gap;
						AddIndex(targetib, n, x, y);
					}
					else if(y==HalfWidth)
					{
						x+=increase_gap;
						AddIndex(targetib, n, x, y);

						x-=increase_gap;
						y+=increase_gap;
						AddIndex(targetib, n, x, y);
					}
					else
					{
						x+=increase_gap;
						AddIndex(targetib, n, x, y);

						x-=increase_gap;
						y+=increase_gap;
						AddIndex(targetib, n, x, y);
					}
				}

				endindex = targetib.GetDynamicIndexDataNumber();

				targetib.Check(D3DPT_TRIANGLESTRIP);

				CheckErrorCCW(targetib, startindex, endindex, (PatchDir)n);

				startpntx += increase_gap;
				startpnty += increase_gap;
				x = startpntx;
				y = startpnty;
			}

			targetib.Upload();
			targetib.DestroyIndexArray();
		}
	}
}


//////////////////////////////////////////////////////////////////////////
// 사용법 (스마트 버텍스 버퍼 사용법과 비슷하다. 참고할것)
// 유의사항 : 내 엔진에서 그대로 뜯어왔으니 약간의 수정을 요함.. 32bit 는 생략했음. 


// Smart16BitIndexBuffer aa(300);

// aa.AddIndex(1);
// aa.AddIndex(2);
// aa.AddIndex(3);
// aa.AddIndex(4);
// aa.Check(D3DPT_TRIANGLESTRIP);	// 지금까지 add 한 인딕스들이 이루는 형태
// aa.AddIndex(5);
// aa.AddIndex(6);
// aa.AddIndex(7);
// aa.AddIndex(6);
// aa.AddIndex(7);
// aa.AddIndex(8);
// aa.Check(D3DPT_TRIANGLELIST);	// 지금까지 add 한 인딕스들이 이루는 형태
// .....
// aa.Upload();						// 다끝나면 system memory buffer 에서 video memory buffer 로 upload

// vv.SetFVF();						// 당연히 소스가 되는 버텍스 버퍼를 설정해 줘야한다
// vv.SetStreamSource();			// 당연히 소스가 되는 버텍스 버퍼를 설정해 줘야한다
// aa.SetIndices();
// aa.Draw();











class Smart16BitIndexBuffer
{
private:
struct IndexCheckInfo
{
IndexCheckInfo();
IndexCheckInfo(D3DPRIMITIVETYPE PrimitiveType_, size_t StartIndexCount_, 
size_t UsedIndexCount_, size_t PrimitiveCount_);

D3DPRIMITIVETYPE PrimitiveType;
size_t StartIndexCount;
size_t UsedIndexCount;
size_t PrimitiveCount;
};

typedef vector<IndexCheckInfo> IndexChickInfoList;
typedef vector<IndexCheckInfo>::iterator IndexChickInfoIter;

public:
Smart16BitIndexBuffer(RenderSystem* pRenderDevice);
virtual ~Smart16BitIndexBuffer(void);

void CreateIndexArray();
void CreateIndexArray(size_t IndexNumber);
void DestroyIndexArray();
void CreateIndexBuffer();
void DestroyIndexBuffer();

void ResetDynamicIndexPtr();
bool SetIndices();

size_t GetIndexNumber() const;
size_t GetIndicesSizeInByte() const;
size_t GetIndexSizeInByte() const;

bool Upload(size_t StartIndex = 0, size_t IndexNumber = 0);
bool Upload(const WORD* TargetData, size_t StartIndex, size_t IndexNumber);
bool Download(WORD* DestPtr, size_t StartIndex, size_t IndexNumber);

__inline void operator ++() { ++DynamicIndexPtr_; }
__inline void operator ++(int) { ++DynamicIndexPtr_; }
__inline void operator --() { DynamicIndexPtr_--; }
__inline void operator +=(size_t how) { DynamicIndexPtr_ += how; }
__inline void operator -=(size_t how) { DynamicIndexPtr_-=how; }
__inline void operator =(size_t when) { DynamicIndexPtr_= IndexArray_ + when; }
__inline WORD& operator [] (size_t Index) { return IndexArray_[Index]; }
__inline const WORD& operator [] (size_t Index) const { return IndexArray_[Index]; }
void Check(D3DPRIMITIVETYPE PrimitiveType);
bool Draw();
size_t GetDynamicIndexDataNumber() const;
__inline void AddIndex(WORD Index);

private:
bool Lock(size_t StartIndex = 0, size_t LockIndexNumber = 0);
bool Unlock();

// todo
// bool DrawUpload()	

private:
LPDIRECT3DINDEXBUFFER9 IndexBuffer_;
IndexChickInfoList IndexCheckInfoList_;
size_t CheckCount_;
size_t IndexNumber_;
WORD* IndexArray_;
WORD* DynamicIndexPtr_;
size_t DynamicIndexCount_;
WORD* LockPtr_;
RenderSystem* m_pRenderSys;
};

__inline void Smart16BitIndexBuffer::AddIndex(WORD Index)
{
*DynamicIndexPtr_ = Index;
++DynamicIndexPtr_;
++DynamicIndexCount_;
++CheckCount_;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////



Smart16BitIndexBuffer::IndexCheckInfo::IndexCheckInfo()
{
}

Smart16BitIndexBuffer::IndexCheckInfo::IndexCheckInfo(
D3DPRIMITIVETYPE PrimitiveType_, size_t StartIndexCount_, 
size_t UsedIndexCount_, size_t PrimitiveCount_)
: PrimitiveType(PrimitiveType_),
StartIndexCount(StartIndexCount_),
UsedIndexCount(UsedIndexCount_),
PrimitiveCount(PrimitiveCount_)
{
}

Smart16BitIndexBuffer::Smart16BitIndexBuffer(RenderSystem* pRenderSys)
: m_pRenderSys(pRenderSys),
LockPtr_(NULL), 
IndexArray_(NULL), 
IndexBuffer_(NULL), 
IndexNumber_(0),
DynamicIndexCount_(0),
CheckCount_(0)
{
IndexCheckInfoList_.reserve(100);
}

Smart16BitIndexBuffer::~Smart16BitIndexBuffer(void)
{
DestroyIndexArray();
DestroyIndexBuffer();
}

void Smart16BitIndexBuffer::CreateIndexArray()
{
if(IndexArray_ == NULL)
{
IndexArray_ = new WORD[IndexNumber_];
}
ResetDynamicIndexPtr();
}

void Smart16BitIndexBuffer::CreateIndexArray(size_t IndexNumber)
{
if(IndexArray_ == NULL)
{
IndexNumber_ = IndexNumber;
IndexArray_ = new WORD[IndexNumber_];
}
ResetDynamicIndexPtr();
}

void Smart16BitIndexBuffer::DestroyIndexArray()
{
SAFE_DELETE_ARRAY(IndexArray_);
}

void Smart16BitIndexBuffer::CreateIndexBuffer()
{
IndexBuffer_ = 
m_pRenderSys->CreateIndexBuffer(sizeof(WORD)*IndexNumber_, D3DFMT_INDEX16);
}

void Smart16BitIndexBuffer::DestroyIndexBuffer()
{
SAFE_RELEASE(IndexBuffer_);
}

bool Smart16BitIndexBuffer::SetIndices()
{
if(IndexBuffer_ == NULL)
return false;

m_pRenderSys->Device()->SetIndices(IndexBuffer_);

return true;
}

bool Smart16BitIndexBuffer::Upload(size_t StartIndex, size_t IndexNumber)
{
if(Lock(StartIndex, IndexNumber) == false)
return false;

if(StartIndex == 0 && IndexNumber == 0)
{
StartIndex = 0;
IndexNumber = IndexNumber_;
}

memcpy(LockPtr_, &IndexArray_[StartIndex], IndexNumber * sizeof(WORD));

if(Unlock() == false)
return false;

return true;
}

bool Smart16BitIndexBuffer::Upload(const WORD* TargetData, size_t StartIndex, size_t IndexNumber)
{
if(IndexArray_)
return false;

memcpy(&IndexArray_[StartIndex], TargetData, IndexNumber * sizeof(WORD));

if(IndexBuffer_)
return false;

if(Lock(StartIndex, IndexNumber) == false)
return false;

if(StartIndex == 0 && IndexNumber == 0)
{
StartIndex = 0;
IndexNumber = IndexNumber_;
}

memcpy(LockPtr_, &IndexArray_[StartIndex], IndexNumber * sizeof(WORD));

if(Unlock() == false)
return false;

return true;
}

bool Smart16BitIndexBuffer::Download(WORD* DestPtr, size_t StartIndex, size_t IndexNumber)
{
if(IndexArray_ == NULL)
return false;

if(StartIndex == 0 && IndexNumber == 0)
{
StartIndex = 0;
IndexNumber = IndexNumber_;
}

memcpy(DestPtr, &IndexArray_[StartIndex], sizeof(WORD)*IndexNumber);

return true;
}

void Smart16BitIndexBuffer::Check(D3DPRIMITIVETYPE PrimitiveType)
{
IndexCheckInfoList_.push_back(	
IndexCheckInfo(	PrimitiveType, 
DynamicIndexCount_,
CheckCount_, 
qb::ComputePrimitiveNumber(PrimitiveType, CheckCount_)));
CheckCount_ = 0;
}

bool Smart16BitIndexBuffer::Draw()
{
if(IndexBuffer_ == NULL)
return false;

if(IndexCheckInfoList_.empty() == true)
return false;

for(IndexChickInfoIter Iter = IndexCheckInfoList_.begin(); Iter != IndexCheckInfoList_.end(); ++Iter)
{
m_pRenderSys->DrawIndexedPrimitive(Iter->PrimitiveType, 0,0,
Iter->UsedIndexCount, Iter->StartIndexCount, Iter->PrimitiveCount);
}

return true;
}

bool Smart16BitIndexBuffer::Lock(size_t StartIndex, size_t LockIndexNumber)
{
if(IndexBuffer_ == NULL)
return false;

if(FAILED(IndexBuffer_->Lock(StartIndex * sizeof(WORD), LockIndexNumber * sizeof(WORD),
(void**)&LockPtr_, 0)))
return false;

return true;
}

void Smart16BitIndexBuffer::ResetDynamicIndexPtr()
{
DynamicIndexPtr_ = IndexArray_;
DynamicIndexCount_ = 0;
CheckCount_ = 0;
IndexCheckInfoList_.resize(0);
}

size_t Smart16BitIndexBuffer::GetIndexNumber() const
{
return IndexNumber_;
}

size_t Smart16BitIndexBuffer::GetIndicesSizeInByte() const
{
return sizeof(WORD) * IndexNumber_;
}

size_t Smart16BitIndexBuffer::GetIndexSizeInByte() const
{
return sizeof(WORD);
}

size_t Smart16BitIndexBuffer::GetDynamicIndexDataNumber() const
{
return DynamicIndexCount_;
}

bool Smart16BitIndexBuffer::Unlock()
{
if(IndexBuffer_ == NULL)
return false;

if(FAILED(IndexBuffer_->Unlock()))
return false;

LockPtr_ = NULL;

return true;
}


*/


/*
{
if(GetCapture())
{
D3DXMATRIXA16 matProj;
m_pd3dDevice->GetTransform( D3DTS_PROJECTION, &matProj );

POINT ptCursor;
GetCursorPos( &ptCursor );
ScreenToClient( m_hWnd, &ptCursor );

// Compute the vector of the pick ray in screen space
D3DXVECTOR3 v;
v.x =  ( ( ( 2.0f * ptCursor.x ) / m_d3dsdBackBuffer.Width  ) - 1 ) / matProj._11;
v.y = -( ( ( 2.0f * ptCursor.y ) / m_d3dsdBackBuffer.Height ) - 1 ) / matProj._22;
v.z =  1.0f;

// Get the inverse of the composite view and world matrix
D3DXMATRIXA16 matView, matWorld, m;
m_pd3dDevice->GetTransform( D3DTS_VIEW, &matView );
m_pd3dDevice->GetTransform( D3DTS_WORLD, &matWorld );

m = matWorld * matView;
D3DXMatrixInverse( &m, NULL, &m );

// Transform the screen space pick ray into 3D space
m_vRayDir.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
m_vRayDir.y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
m_vRayDir.z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
m_vRayPos.x = m._41;
m_vRayPos.y = m._42;
m_vRayPos.z = m._43;

return S_OK;
}

return E_FAIL;	
}
*/