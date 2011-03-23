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
		m_matWorld = GetParent()->GetWorldTransform() * m_matLocal;

	//virtual
	vInitGeometry(fElapsedtime);
	
	m_DXmatWorld._11 = m_matWorld.m_Rotate.GetEntry( 0 , 0 ) * m_matWorld.m_fScale;
	m_DXmatWorld._12 = m_matWorld.m_Rotate.GetEntry( 0 , 1 ) * m_matWorld.m_fScale;
	m_DXmatWorld._13 = m_matWorld.m_Rotate.GetEntry( 0 , 2 ) * m_matWorld.m_fScale;
	m_DXmatWorld._14 = 0.0f;

	m_DXmatWorld._21 = m_matWorld.m_Rotate.GetEntry( 1 , 0 ) * m_matWorld.m_fScale;
	m_DXmatWorld._22 = m_matWorld.m_Rotate.GetEntry( 1 , 1 ) * m_matWorld.m_fScale;
	m_DXmatWorld._23 = m_matWorld.m_Rotate.GetEntry( 1 , 2 ) * m_matWorld.m_fScale;
	m_DXmatWorld._24 = 0.0f;

	m_DXmatWorld._31 = m_matWorld.m_Rotate.GetEntry( 2 , 0 ) * m_matWorld.m_fScale;
	m_DXmatWorld._32 = m_matWorld.m_Rotate.GetEntry( 2 , 1 ) * m_matWorld.m_fScale;
	m_DXmatWorld._33 = m_matWorld.m_Rotate.GetEntry( 2 , 2 ) * m_matWorld.m_fScale;
	m_DXmatWorld._34 = 0.0f;

	m_DXmatWorld._41 = m_matWorld.m_Translate[0];
	m_DXmatWorld._42 = m_matWorld.m_Translate[1];
	m_DXmatWorld._43 = m_matWorld.m_Translate[2];
	m_DXmatWorld._44 = 1.0f;

	GetDevice()->SetTransform( D3DTS_WORLD, &m_DXmatWorld );
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

