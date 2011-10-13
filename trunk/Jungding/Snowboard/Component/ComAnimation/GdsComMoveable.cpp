#include "stdafx.h"

GdsComMoveable::GdsComMoveable()
:m_vTranslate(0.0f, 0.0f, 0.0f)
,m_vWorldTranslate(0.0f, 0.0f, 0.0f)
,m_vScale( 1.0f, 1.0f, 1.0f)
{
	D3DXMatrixIdentity( &m_matWorld );
	D3DXMatrixIdentity( &m_matLocal );
	D3DXQuaternionIdentity(&m_qWorldRotate);
	D3DXQuaternionIdentity(&m_qRotate);
}

GdsComMoveable::~GdsComMoveable()
{

}

void GdsComMoveable::Make()
{

}

void GdsComMoveable::Update( float fAccumtime )
{
	GdsGameObject* pObject = GetOwnerGO();
	if ( pObject == NULL )
	{
		return;
	}

	D3DXMATRIX matTrans, matScale, matRot;
	D3DXMatrixIdentity( &matTrans );
	D3DXMatrixIdentity( &matScale );
	D3DXMatrixIdentity( &matRot );

	D3DXMatrixTranslation(&matTrans, m_vTranslate.x, m_vTranslate.y, m_vTranslate.z);
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixRotationQuaternion(&matRot, &m_qRotate);

	m_matLocal = matTrans * matRot * matScale;

	if( pObject->GetParent() )
	{
		D3DXMATRIX parTM = pObject->GetParent()->GetMatrix();
		//m_matWorld = m_matLocal * m_matAni * m_pParent->GetWorldMatrix();
		m_matWorld = m_matLocal * parTM;
	}
	else
	{
		m_matWorld = m_matLocal;
	}

	m_vWorldTranslate = D3DXVECTOR3(m_matWorld._41, m_matWorld._42, m_matWorld._43 ) ;
	D3DXQuaternionRotationMatrix(&m_qWorldRotate, &m_matWorld);

	pObject->SetMatrix( m_matWorld );
}