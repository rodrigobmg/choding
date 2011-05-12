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
, m_iLimitedCountOfFacePerNode( 300 )
, m_bShowBox( false )
, m_bShowAxis( false )
, m_bShowOctreenodeBox( false )
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
		if( ( pI->Lock( 0 , 0 , (void**)&pIB , 0 ) ) == false )
		{
			int iMaxCountIndex = genTriIndex( m_pOctreeRootNode , pIB , 0 );
			rendertoken->SetIndexMaxCount( iMaxCountIndex );
		}		
		pI->Unlock();
	}
}

void GdsNode::drawAxis()
{
	if ( RENDERER.GetDevice() == NULL )
		return;

	D3DXMATRIXA16 matWorld;
	D3DXMATRIXA16 matView;
	D3DXMATRIXA16 matProj;
	RENDERER.GetDevice()->GetTransform( D3DTS_WORLD , &matWorld );
	RENDERER.GetDevice()->GetTransform( D3DTS_VIEW  , &matView );
	RENDERER.GetDevice()->GetTransform( D3DTS_PROJECTION , &matProj );
	D3DXMATRIXA16 mat = matWorld*matView*matProj;

	D3DXVECTOR3 lineLBN[2];
	lineLBN[0].x = 0.0f; lineLBN[0].y = 0.0f; lineLBN[0].z = 0.0f;
	lineLBN[1].x = 10.0f; lineLBN[1].y = 0.0f; lineLBN[1].z = 0.0f;

	D3DXVECTOR3 lineLTN[2];
	lineLTN[0].x = 0.0f; lineLTN[0].y = 0.0f; lineLTN[0].z = 0.0f;
	lineLTN[1].x = 0.0f; lineLTN[1].y = 10.0f; lineLTN[1].z = 0.0f;

	D3DXVECTOR3 lineLBF[2];
	lineLBF[0].x = 0.0f; lineLBF[0].y = 0.0f; lineLBF[0].z = 0.0f;
	lineLBF[1].x = 0.0f; lineLBF[1].y = 0.0f; lineLBF[1].z = 10.0f;


	ID3DXLine* Line;
	D3DXCreateLine( RENDERER.GetDevice() , &Line );
	Line->SetWidth( 1 );
	Line->SetAntialias( true );
	Line->Begin();
	Line->DrawTransform( lineLBN , 2, &mat, D3DXCOLOR( 1.0f , 0.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineLTN , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineLBF , 2, &mat, D3DXCOLOR( 0.0f , 0.0f , 1.0f , 1.0f ));
	Line->End();
	Line->Release();
}

void GdsNode::drawBoxLine( D3DXVECTOR3& minPos , D3DXVECTOR3& maxPos )
{
	if ( RENDERER.GetDevice() == NULL )
		return;

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
	Line->End();
	Line->Release();	
}

int GdsNode::genTriIndex( Node* node , LPVOID pIB , int iCurIndexCount )
{
	if ( CAMMGR.GetCurCam()->GetFrustum().VertexIsInFrustum( node->m_cenPos ) == false )
		return iCurIndexCount;

	if ( node->m_iCountOfFace > 0 )
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

	if ( m_bShowOctreenodeBox )
		drawBoxLine( node->m_minPos , node->m_maxPos );

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
		if (  ( vb->Lock( 0 , rendertoken->GetVertexMaxCount() * sizeof( GDSVERTEX ) , (void**)&pVB , 0 ) ) == false )
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
	}
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


