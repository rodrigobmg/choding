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
	m_eCull = CULL_OFF;	
}

GdsNode::~GdsNode()
{
	vClear();	
}

void GdsNode::vClear()
{
	RemoveAllChild();
	RENDER_OBJECT_CONTAINER::iterator it = m_list_RenderObject.begin();
	for ( ; it != m_list_RenderObject.end() ; ++it )
	{
		RENDERER.GetRenderFrame()->DetachRenderObject( *it );
	}
	m_list_RenderObject.clear();
}

D3DXVECTOR3& GdsNode::GetTranslate()
{
	return m_vTranslate;
}

void GdsNode::SetTranslate( const D3DXVECTOR3& vPos )
{
	m_vTranslate = vPos;
}

void GdsNode::SetTranslate( float fX, float fY, float fZ)
{
	m_vTranslate = D3DXVECTOR3( fX, fY, fZ );
}

void GdsNode::SetRotate( const D3DXQUATERNION& qRot )
{
	m_qRotate = qRot;
}

void GdsNode::SetRotate( const D3DXVECTOR3& vAxis, float fAngle )
{
	D3DXQuaternionRotationAxis( &m_qRotate, &vAxis, fAngle);
}

const D3DXVECTOR3& GdsNode::GetScale() const
{	
	return m_vScale;
}

void GdsNode::SetScale( float fScale )
{
	assert( fScale >= 0.0f );
	float f = fabs(fScale);
	m_vScale = D3DXVECTOR3( f, f, f);
}

void GdsNode::SetScale(float fScaleX, float fScaleY, float fScaleZ)
{
	assert( fScaleX >= 0.0f);
	assert( fScaleY >= 0.0f);
	assert( fScaleZ >= 0.0f);

	float fX = fabs(fScaleX);
	float fY = fabs(fScaleY);
	float fZ = fabs(fScaleZ);

	m_vScale = D3DXVECTOR3( fX, fY, fZ);
}

const D3DXVECTOR3& GdsNode::GetWorldTranslate() const
{
	return m_vWorldTranslate;
}

const D3DXQUATERNION& GdsNode::GetWorldRotate() const
{
	return m_qWorldRotate;
}

const D3DXMATRIX& GdsNode::GetLocalMatrix() const
{
	return m_matLocal;
}

const D3DXMATRIX& GdsNode::GetWorldMatrix() const
{
	return m_matWorld;
}

void GdsNode::SetLocalMatrix( const D3DXMATRIX& matLocal )
{
	m_matLocal = matLocal;
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


