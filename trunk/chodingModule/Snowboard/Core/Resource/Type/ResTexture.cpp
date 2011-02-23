#include "ResTexture.h"

ImplementBoostPool( CResTexture )

CResTexture::CResTexture()
{
	SetName( OBJECT_RES_TEXTURE );
	Clear();
}

CResTexture::~CResTexture()
{
	if ( m_pTexture )
		m_pTexture->Release();	
}

void CResTexture::Clear()
{
	m_pTexture = NULL;
}

HRESULT CResTexture::Create()
{
	return S_OK;
}

HRESULT CResTexture::Release()
{	

	DecRefCount();
	return S_OK;
}