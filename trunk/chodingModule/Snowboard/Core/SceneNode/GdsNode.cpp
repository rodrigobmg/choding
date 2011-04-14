#include "GdsNode.h"

//ImplementBoostPool( GdsNode )
//boost::pool<> GdsNode::bpool( sizeof( GdsNode ) );

GdsNode::GdsNode():
m_bDrawAxis( false )
{
	SetName( OBJECT_NODE );
	m_ChildNode.clear();
	m_pParentNode = NULL;
//	m_pParentNode = GdsNodePtr( (GdsNode*)NULL );
	m_matWorld.MakeIdentity();
	m_matLocal.MakeIdentity();
	m_eCull = CULL_OFF;	
}

GdsNode::~GdsNode()
{
	vClear();
	RemoveAllChild();
}

void GdsNode::vClear()
{

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
//void GdsNode::SetParent( GdsNodePtr pNode )
{
	m_pParentNode = pNode;
}

GdsNode*	GdsNode::GetParent()
//GdsNodePtr	GdsNode::GetParent()
{
	return m_pParentNode;
}

// 0���� ������
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

	ASSERT( 0 && L"��ã�°� ���� �ȵȴ�. ���ϵ� ����Ʈ�� ������ �ڽĳ�带 ã���߿� �� ������ ��尡 �������߿� �������Ŵ�;; ");
	return GdsNodePtr( (GdsNode*)NULL );
}


HRESULT GdsNode::AttachChild( GdsNodePtr pNode )
{
	if ( pNode == NULL )
		return false;

	//pNode->SetParent( shared_ptr_this() );	
	//pNode->SetParent( GdsNodePtr( ( GdsNode*)this ) );	
	pNode->SetParent( pNode.get() );	
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
	
	if ( GetParent() == NULL )
		m_matWorld = m_matLocal;
	else
		m_matWorld = GetParent()->GetWorldTransform() * m_matLocal;

	m_DXmatWorld._11 = m_matWorld.m_Rotate.GetEntry( 0 , 0 ) * m_matWorld.m_fScale;
	m_DXmatWorld._12 = m_matWorld.m_Rotate.GetEntry( 1 , 0 ) * m_matWorld.m_fScale;
	m_DXmatWorld._13 = m_matWorld.m_Rotate.GetEntry( 2 , 0 ) * m_matWorld.m_fScale;
	m_DXmatWorld._14 = 0.0f;

	m_DXmatWorld._21 = m_matWorld.m_Rotate.GetEntry( 0 , 1 ) * m_matWorld.m_fScale;
	m_DXmatWorld._22 = m_matWorld.m_Rotate.GetEntry( 1 , 1 ) * m_matWorld.m_fScale;
	m_DXmatWorld._23 = m_matWorld.m_Rotate.GetEntry( 2 , 1 ) * m_matWorld.m_fScale;
	m_DXmatWorld._24 = 0.0f;

	m_DXmatWorld._31 = m_matWorld.m_Rotate.GetEntry( 0 , 2 ) * m_matWorld.m_fScale;
	m_DXmatWorld._32 = m_matWorld.m_Rotate.GetEntry( 1 , 2 ) * m_matWorld.m_fScale;
	m_DXmatWorld._33 = m_matWorld.m_Rotate.GetEntry( 2 , 2 ) * m_matWorld.m_fScale;
	m_DXmatWorld._34 = 0.0f;

	m_DXmatWorld._41 = m_matWorld.m_Translate[0];
	m_DXmatWorld._42 = m_matWorld.m_Translate[1];
	m_DXmatWorld._43 = m_matWorld.m_Translate[2];
	m_DXmatWorld._44 = 1.0f;

	vUpdateGeometry( fElapsedtime );

	if ( !m_ChildNode.empty() )
	{
		for( CHILDNODE_CONTAINER::iterator it = m_ChildNode.begin() ; it != m_ChildNode.end() ; ++it )
		{
			(*it)->Update(fElapsedtime);
		}
	}	

	return TRUE;
}


void GdsNode::vUpdateGeometry( float fElapsedtime )
{
	
}

void GdsNode::DrawAxis()
{
// 	D3DXMATRIXA16 matWorld;
// 	D3DXMATRIXA16 matView;
// 	D3DXMATRIXA16 matProj;
// 	m_Device->GetTransform( D3DTS_WORLD , &matWorld );
// 	m_Device->GetTransform( D3DTS_VIEW  , &matView );
// 	m_Device->GetTransform( D3DTS_PROJECTION , &matProj );
// 
// 	D3DXVECTOR3 axisX[2];
// 	axisX[0].x = 0.0f; axisX[0].y = 0.0f; axisX[0].z = 0.0f;
// 	axisX[1].x = 10.0f; axisX[1].y = 0.0f; axisX[1].z = 0.0f;
// 
// 	D3DXVECTOR3 axisY[2];
// 	axisY[0].x = 0.0f; axisY[0].y = 0.0f; axisY[0].z = 0.0f;
// 	axisY[1].x = 0.0f; axisY[1].y = 10.0f; axisY[1].z = 0.0f;
// 
// 	D3DXVECTOR3 axisZ[2];
// 	axisZ[0].x = 0.0f; axisZ[0].y = 0.0f; axisZ[0].z = 0.0f;
// 	axisZ[1].x = 0.0f; axisZ[1].y = 0.0f; axisZ[1].z = 10.0f;
// 
// 	//D3DXMatrixIdentity( &matWorld );
// 
// 	ID3DXLine* Line;
// 	D3DXCreateLine( m_Device , &Line );
// 	Line->SetWidth( 1 );
// 	Line->SetAntialias( true );
// 	Line->Begin();
// 	Line->DrawTransform( axisX , 2, &(matWorld*matView*matProj), D3DXCOLOR( 1.0f , 0.0f , 0.0f , 1.0f ));
// 	Line->DrawTransform( axisY , 2, &(matWorld*matView*matProj), D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
// 	Line->DrawTransform( axisZ , 2, &(matWorld*matView*matProj), D3DXCOLOR( 0.0f , 0.0f , 1.0f , 1.0f ));
// 	Line->End();
// 	Line->Release();
}

