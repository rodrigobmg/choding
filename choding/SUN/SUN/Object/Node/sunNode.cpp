#include "Precompiled.h"
#include "sunNode.h"



ImplementRootRTTI( sunNode );


sunNode::sunNode():
 m_pParent(NULL),
 
 m_vPos(0.0f, 0.0f, 0.0f),
 m_vWorldPos(0.0f, 0.0f, 0.0f),
 m_vScale( 1.0f, 1.0f, 1.0f)

{
	m_ChildList.clear();
	D3DXMatrixIdentity(&m_matLocal);
	D3DXMatrixIdentity(&m_matAni);
	D3DXMatrixIdentity(&m_matWorld);
	D3DXQuaternionIdentity(&m_qWorldRot);
	D3DXQuaternionIdentity(&m_qRot);
}

sunNode::~sunNode()
{
	Destroy();
}

void sunNode::Destroy()
{
	
	m_pParent = NULL;
	RemoveAllChild();

}

inline const D3DXVECTOR3& sunNode::GetTranslate()
{
	return m_vPos;
}

inline void sunNode::SetTranslate( const D3DXVECTOR3& vPos )
{
	m_vPos = vPos;
}

inline void sunNode::SetTranslate( float fX, float fY, float fZ)
{
	m_vPos = D3DXVECTOR3( fX, fY, fZ );
}



inline void sunNode::SetRotate( const D3DXQUATERNION& qRot )
{
	m_qRot = qRot;
}

inline void sunNode::SetRotate( const D3DXVECTOR3& vAxis, float fAngle )
{
	D3DXQuaternionRotationAxis( &m_qRot, &vAxis, fAngle);
}

inline const D3DXVECTOR3& sunNode::GetScale() const
{	
	return m_vScale;
}

inline void sunNode::SetScale( float fScale )
{
	assert( fScale >= 0.0f );
	float f = fabs(fScale);
	m_vScale = D3DXVECTOR3( f, f, f);
}

inline void sunNode::SetScale(float fScaleX, float fScaleY, float fScaleZ)
{
	assert( fScaleX >= 0.0f);
	assert( fScaleY >= 0.0f);
	assert( fScaleZ >= 0.0f);

	float fX = fabs(fScaleX);
	float fY = fabs(fScaleY);
	float fZ = fabs(fScaleZ);

	m_vScale = D3DXVECTOR3( fX, fY, fZ);
}

inline const D3DXVECTOR3& sunNode::GetWorldTranslate() const
{
	return m_vWorldPos;
}

inline const D3DXQUATERNION& sunNode::GetWorldRotate() const
{
	return m_qWorldRot;
}

const D3DXMATRIX& sunNode::GetLocalMatrix() const
{
	return m_matLocal;
}

const D3DXMATRIX& sunNode::GetWorldMatrix() const
{
	return m_matWorld;
}

void sunNode::SetLocalMatrix( const D3DXMATRIX& matLocal )
{
	m_matLocal = matLocal;
}

void sunNode::SetLocalMatrix( const _stMat16& kMat4X4 )
{
	memcpy( &m_matLocal, &kMat4X4, sizeof(m_matLocal) );
}

void sunNode::SetWorldMatrix( const _stMat16& kMat4X4 )
{
	memcpy( &m_matWorld, &kMat4X4, sizeof(m_matWorld) );
}

inline void sunNode::SetLocalFromWorldTransform( const D3DXMATRIX& matWorld )
{
	if(m_pParent)
	{
		D3DXMATRIX matParentWorldInv;
		D3DXMatrixInverse( &matParentWorldInv, NULL, &m_pParent->GetWorldMatrix());
		D3DXMATRIX matLocal = matParentWorldInv * matWorld;
		SetLocalMatrix( matLocal );

	}
	else
	{
		SetLocalMatrix( matWorld );
	}

}

void sunNode::BegineUpdate()
{
}

void sunNode::Update()
{
}

void sunNode::EndUpdate()
{
}

void sunNode::UpdateWorldData()
{
	D3DXMATRIX matTrans, matScale, matRot;
	D3DXMatrixIdentity( &matTrans );
	D3DXMatrixIdentity( &matScale );
	D3DXMatrixIdentity( &matRot );

	D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, m_vPos.z);
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixRotationQuaternion(&matRot, &m_qRot);

	//m_matLocal = matTrans * matRot * matScale;

	if(m_pParent)
	{
		m_matWorld = m_matLocal * m_matAni * m_pParent->GetWorldMatrix();
	}
	else
	{
		m_matWorld = m_matLocal;
	}

	m_vWorldPos = D3DXVECTOR3(m_matWorld._41, m_matWorld._42, m_matWorld._43 ) ;
	D3DXQuaternionRotationMatrix(&m_qWorldRot, &m_matWorld);

}

void sunNode::UpdateListner()
{
	BegineUpdate();
	Update();
	UpdateWorldData();
	EndUpdate();

	
	ChildIT IT = m_ChildList.begin();

	for( ; IT != m_ChildList.end(); ++IT)
	{
		sunNodePtr spNodePtr = (*IT);
		
		assert(NULL != spNodePtr);
		
		spNodePtr->UpdateListner();
		
	}


}



//
//inline void sunNode::AddChild( sunNodePtr spChild )
//{
//	assert(spChild);
//	spChild->SetParent(this);
//	m_ChildList.push_back( spChild );
//}
//
//inline void sunNode::AddChildToFront( sunNodePtr spChild )
//{
//	assert( spChild );
//	spChild->SetParent( this );
//	m_ChildList.push_back( spChild );
//
//}

inline sunNodePtr sunNode::GetNodeByName( const tstring& strName, bool bSearchAllChildren /* = true */)
{	
	if( strName.length() < 1)
		return NULL;

	if( true == IsName( strName))
		return this;

	ChildIT IT = m_ChildList.begin();

	for( ; IT != m_ChildList.end(); ++IT)
	{
		sunNodePtr spChildPtr = (*IT);
		assert(spChildPtr);

		if( bSearchAllChildren)
		{
			sunNodePtr spNode = spChildPtr->GetNodeByName( strName );
			if( spNode )
				return spNode;
		}
		else
		{
			if( spChildPtr->IsName( strName))
				return spChildPtr;
		}
		
	}

	return NULL;
}

void sunNode::RemoveChild( sunNodePtr spNode )
{
	ChildIT it = m_ChildList.begin();
	while( it != m_ChildList.end())
	{
		if( (*it) == spNode )
		{
			(*it)->SetParent(NULL);
			m_ChildList.erase( it );
			break;
		}
		++it;
	}

}

void sunNode::RemoveAllChild()
{
	ChildIT it = m_ChildList.begin();


	for(; it != m_ChildList.end(); ++it)
	{
		sunNodePtr spChildPtr = (*it);
		assert(spChildPtr);

		spChildPtr->SetParent(NULL);
	}
		
	m_ChildList.clear();
}

