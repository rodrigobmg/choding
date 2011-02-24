#include "GdsNode.h"

GdsNode::GdsNode():
m_pParentNode(NULL)
{
	SetName( OBJECT_NODE_BASE );
}

GdsNode::~GdsNode()
{
	DecRefCount();
}

HRESULT GdsNode::RemoveAllChild()
{
// 	if ( !m_listChildNode.empty() )
// 		for_each( m_listChildNode.begin() , m_listChildNode.end() , functor::removeallchild() );

	if ( !m_listChildNode.empty() )
	{
		CHILDLIST::iterator it = m_listChildNode.begin();
		for( ; it != m_listChildNode.end() ;  )
		{
			(*it)->RemoveAllChild();
			SAFE_DELETE( (*it) );
			m_listChildNode.erase( it++ );
		}
	}
	

	return TRUE;
}

HRESULT GdsNode::Update( float fElapsedtime )
{
	vUpdate( fElapsedtime );
	if ( !m_listChildNode.empty() )
	{
		for( CHILDLIST::iterator it = m_listChildNode.begin() ; it != m_listChildNode.end() ; ++it )
		{
			(*it)->Update(fElapsedtime);
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

// 0���� ������
GdsNode*	GdsNode::GetAt( unsigned int index )
{
	if ( m_listChildNode.empty() )
		return NULL;

	if ( m_listChildNode.size() < index )
		return NULL;

 	CHILDLIST::iterator it = m_listChildNode.begin();
 	for ( size_t t = 0 ; it != m_listChildNode.end() ; ++it )
 	{
		if ( t == index )
			return (*it);

 		++t;
 	}

	assert( 0 && "��ã�°� ���� �ȵȴ�. ���ϵ� ����Ʈ�� ������ �ڽĳ�带 ã���߿� �� ������ ��尡 �������߿� �������Ŵ�;; ");
	return NULL;
}


HRESULT GdsNode::AttachChild( GdsNode* pNode )
{
	if ( this == pNode )
		return S_FALSE;

	pNode->SetParent( this );
	
	pNode->IncRefCount();

	m_listChildNode.push_back( pNode );
	return S_OK;
}

HRESULT GdsNode::DetachChild( GdsNode* pNode )
{
	if ( this == pNode )
		return S_FALSE;

	CHILDLIST::iterator it = m_listChildNode.begin();
	for( ; it != m_listChildNode.end() ; ++it )
	{
		if ( (*it) == pNode )
		{
			(*it)->DecRefCount();
			m_listChildNode.erase( it );
			break;
		}
	}

	return S_OK;
}

HRESULT GdsNode::vUpdate( float fElapsedtime )
{
	++fElapsedtime;
	//Do something
	return S_OK;
}
