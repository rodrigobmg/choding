#include "GdsNode.h"

//ImplementBoostPool( GdsNode )
//boost::pool<> GdsNode::bpool( sizeof( GdsNode ) );

GdsNode::GdsNode():
m_bDrawAxis( false )
{
	SetName( OBJECT_NODE );
	m_ChildNode.clear();
	m_Device = NULL;
	m_pParentNode = GdsNodePtr( (GdsNode*)NULL );
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

void GdsNode::SetParent( GdsNodePtr pNode )
{
	m_pParentNode = pNode;
}

GdsNodePtr	GdsNode::GetParent()
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
	if ( this == pNode.get() )
		return false;

	pNode->SetParent( shared_ptr_this() );	
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

	return true;
}

HRESULT GdsNode::Update( float fElapsedtime )
{
	if ( m_Device == NULL )
		m_Device = GetParent()->GetDevice();

	if ( GetParent() != NULL && ( m_Device != GetParent()->GetDevice() ) )
		m_Device = GetParent()->GetDevice();
	
	InitGeometry( fElapsedtime );
	Render( fElapsedtime );

	if ( !m_ChildNode.empty() )
	{
		for( CHILDNODE_CONTAINER::iterator it = m_ChildNode.begin() ; it != m_ChildNode.end() ; ++it )
		{
			(*it)->Update(fElapsedtime);
		}
	}

	return TRUE;
}


void GdsNode::InitGeometry( float fElapsedtime )
{
	if ( GetParent() == NULL )
		m_matWorld = m_matLocal;
	else
		m_matWorld = m_matLocal * GetParent()->GetWorldTransform();

	//virtual
	vInitGeometry(fElapsedtime);
	
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity( &matWorld );

	m_matWorld.m_Rotate.GetRow( 0 , matWorld._11 , matWorld._12 , matWorld._13 );
	m_matWorld.m_Rotate.GetRow( 1 , matWorld._21 , matWorld._22 , matWorld._23 );
	m_matWorld.m_Rotate.GetRow( 2 , matWorld._31 , matWorld._32 , matWorld._33 );
	matWorld._41 = m_matWorld.m_Translate[0]; 
	matWorld._42 = m_matWorld.m_Translate[1];
	matWorld._43 = m_matWorld.m_Translate[2];	
	matWorld._11 *= m_matWorld.m_fScale;
	matWorld._22 *= m_matWorld.m_fScale;
	matWorld._33 *= m_matWorld.m_fScale;

	GetDevice()->SetTransform( D3DTS_WORLD, &matWorld );
}

void GdsNode::Render( float fElapsedtime )
{
	if ( m_bDrawAxis )
		DrawAxis();

	if ( m_eCull == CULL_ON )
		return;

	if ( GetDevice() == NULL )
		return;

	if ( GetPropertyState() == NULL )
		return;


	GetPropertyState()->Render( GetDevice() );


	// virtual
	vRender(fElapsedtime);
}

void GdsNode::vInitGeometry( float fElapsedtime )
{
}

void GdsNode::vRender( float fElapsedtime )
{	
}

void GdsNode::DrawAxis()
{
	D3DXMATRIXA16 matWorld;
	D3DXMATRIXA16 matView;
	D3DXMATRIXA16 matProj;
	m_Device->GetTransform( D3DTS_WORLD , &matWorld );
	m_Device->GetTransform( D3DTS_VIEW  , &matView );
	m_Device->GetTransform( D3DTS_PROJECTION , &matProj );

	D3DXVECTOR3 axisX[2];
	axisX[0].x = 0.0f; axisX[0].y = 0.0f; axisX[0].z = 0.0f;
	axisX[1].x = 10.0f; axisX[1].y = 0.0f; axisX[1].z = 0.0f;

	D3DXVECTOR3 axisY[2];
	axisY[0].x = 0.0f; axisY[0].y = 0.0f; axisY[0].z = 0.0f;
	axisY[1].x = 0.0f; axisY[1].y = 10.0f; axisY[1].z = 0.0f;

	D3DXVECTOR3 axisZ[2];
	axisZ[0].x = 0.0f; axisZ[0].y = 0.0f; axisZ[0].z = 0.0f;
	axisZ[1].x = 0.0f; axisZ[1].y = 0.0f; axisZ[1].z = 10.0f;

	//D3DXMatrixIdentity( &matWorld );

	ID3DXLine* Line;
	D3DXCreateLine( m_Device , &Line );
	Line->SetWidth( 1 );
	Line->SetAntialias( true );
	Line->Begin();
	Line->DrawTransform( axisX , 2, &(matWorld*matView*matProj), D3DXCOLOR( 1.0f , 0.0f , 0.0f , 1.0f ));
	Line->DrawTransform( axisY , 2, &(matWorld*matView*matProj), D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
	Line->DrawTransform( axisZ , 2, &(matWorld*matView*matProj), D3DXCOLOR( 0.0f , 0.0f , 1.0f , 1.0f ));
	Line->End();
	Line->Release();
}

