#include "ResTexture.h"

CResTexture::CResTexture()
{
	SetName( OBJECT_RES_TEXTURE );
	Clear();
}

CResTexture::~CResTexture()
{
	
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
	if ( m_pTexture )
		m_pTexture->Release();

	DecRefCount();
	return S_OK;
}