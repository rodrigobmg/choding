#include "GdsNode.h"

GdsNode::GdsNode()
{
	SetName( OBJECT_NODE );
	m_ChildNode.clear();
	m_Device = NULL;
	m_pParentNode = GdsNodePtr( (GdsNode*)NULL );
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
