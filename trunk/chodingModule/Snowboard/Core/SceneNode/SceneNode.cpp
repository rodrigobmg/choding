#include "SceneNode.h"

SceneNode::SceneNode():
m_pParentNode(NULL)
{
	SetName( OBJECT_SCENENODE_BASE );
}

SceneNode::~SceneNode()
{
	DecRefCount();
}

HRESULT SceneNode::RemoveAllChild()
{
	if ( !m_listChildNode.empty() )
		for_each( m_listChildNode.begin() , m_listChildNode.end() , functor::removeallchild() );

	return TRUE;
}

HRESULT SceneNode::Update( float fElapsedtime )
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


void SceneNode::SetParent( SceneNode* pNode )
{
	m_pParentNode = pNode;
}

SceneNode*	SceneNode::GetParent()
{
	return m_pParentNode;
}

// 0부터 시작함
SceneNode*	SceneNode::GetAt( unsigned int index )
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

	assert( 0 && "못찾는건 말이 안된다. 차일드 리스트의 마지막 자식노드를 찾는중에 그 마지막 노드가 루프도중에 지워진거다;; ");
	return NULL;
}


HRESULT SceneNode::AttachChild( SceneNode* pNode )
{
	if ( this == pNode )
		return S_FALSE;

	pNode->SetParent( this );
	
	pNode->IncRefCount();

	m_listChildNode.push_back( pNode );
	return S_OK;
}

HRESULT SceneNode::DetachChild( SceneNode* pNode )
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

HRESULT SceneNode::vUpdate( float fElapsedtime )
{
	++fElapsedtime;
	//Do something
	return S_OK;
}
