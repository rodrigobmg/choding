#include "GdsNode.h"

GdsNode::GdsNode()
{
	SetName( OBJECT_NODE );
	m_listChildNode.clear();
	m_Device = NULL;
	m_pParentNode = GdsNodePtr( (GdsNode*)NULL );
	m_pResource = GdsResBasePtr( (GdsResBase*)NULL );
	m_matWorld.MakeIdentity();
	m_matLocal.MakeIdentity();
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
	if ( !m_listChildNode.empty() )
	{
		CHILDLIST::iterator it = m_listChildNode.begin();
		for( ; it != m_listChildNode.end() ;  )
		{
			(*it)->RemoveAllChild();
			m_listChildNode.erase( it++ );
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
	if ( m_listChildNode.empty() )
		return GdsNodePtr( (GdsNode*)NULL );

	if ( m_listChildNode.size() < index )
		return GdsNodePtr( (GdsNode*)NULL );

 	CHILDLIST::iterator it = m_listChildNode.begin();
 	for ( size_t t = 0 ; it != m_listChildNode.end() ; ++it )
 	{
		if ( t == index )
			return (*it);

 		++t;
 	}

	assert( 0 && L"못찾는건 말이 안된다. 차일드 리스트의 마지막 자식노드를 찾는중에 그 마지막 노드가 루프도중에 지워진거다;; ");
	return GdsNodePtr( (GdsNode*)NULL );
}


HRESULT GdsNode::AttachChild( GdsNodePtr pNode )
{
	if ( this == pNode.get() )
		return false;

	pNode->SetParent( shared_ptr_this() );	
	m_listChildNode.push_back( pNode );
	return true;
}

HRESULT GdsNode::DetachChild( GdsNodePtr pNode )
{
	if ( this == pNode.get() )
		return false;

	CHILDLIST::iterator it = m_listChildNode.begin();
	for( ; it != m_listChildNode.end() ; ++it )
	{
		if ( (*it) == pNode )
		{
			m_listChildNode.erase( it );
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

	if ( !m_listChildNode.empty() )
	{
		for( CHILDLIST::iterator it = m_listChildNode.begin() ; it != m_listChildNode.end() ; ++it )
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

	vInitGeometry(fElapsedtime);
	
}

void GdsNode::Render( float fElapsedtime )
{
	vRender(fElapsedtime);
}

void GdsNode::vInitGeometry( float fElapsedtime )
{
}

void GdsNode::vRender( float fElapsedtime )
{
}