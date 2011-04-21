#include "GdsNode.h"
#include "Camera\GdsCameraManagerDX9.h"
#include "Renderer\GdsRendererDX9.h"

//ImplementBoostPool( GdsNode )
//boost::pool<> GdsNode::bpool( sizeof( GdsNode ) );

GdsNode::GdsNode():
m_bBillboard( false ),
m_Property( new GdsProperty )
{
	SetName( OBJECT_NODE );
	m_ChildNode.clear();
	m_pParentNode = NULL;
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

	vUpdate( fElapsedtime );

	m_Property->GetMesh()->SetMT( m_DXmatWorld );
	RENDERER.AddRenderToken( m_Property );

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
		int32_t cur_cam_index = CAMMGR.GetCurCam();
		GdsCameraNodePtr pCamera = CAMMGR.GetCamNode( cur_cam_index );
		GdsMatrix3 billboard;
		billboard.MakeIdentity();
		GdsMatrix3 camMat = pCamera->GetWorldTransform().m_Rotate;
		billboard.SetEntry( 0 , 0 , camMat.m_pEntry[0][0] );
		billboard.SetEntry( 0 , 2 , camMat.m_pEntry[0][2] );
		billboard.SetEntry( 2 , 0 , camMat.m_pEntry[2][0] );
		billboard.SetEntry( 2 , 2 , camMat.m_pEntry[2][2] );
		billboard = billboard.Inverse();

		GetWorldRotate() = billboard;
	}	
}


