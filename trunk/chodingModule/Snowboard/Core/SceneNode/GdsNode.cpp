#include "GdsNode.h"

ImplementBoostPool( GdsNode )

GdsNode::GdsNode()
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

	//virtual
	vInitGeometry(fElapsedtime);


	GetDevice()->SetTransform( D3DTS_WORLD, &matWorld );
}

void GdsNode::Render( float fElapsedtime )
{
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
