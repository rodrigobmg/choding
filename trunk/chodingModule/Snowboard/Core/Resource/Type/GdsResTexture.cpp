#include "GdsResTexture.h"

ImplementBoostPool( GdsResTexture )

GdsResTexture::GdsResTexture()
{
	SetName( OBJECT_RES_TEXTURE );
	Clear();
}

GdsResTexture::~GdsResTexture()
{
	if ( m_pTexture )
		m_pTexture->Release();	
}

void GdsResTexture::Clear()
{
	m_pTexture = NULL;
}

HRESULT GdsResTexture::Create()
{
	return S_OK;
}

HRESULT GdsResTexture::Release()
{	
	DecRefCount();
	return S_OK;
}