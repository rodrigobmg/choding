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
	
	Clear();
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
	return S_OK;
}

HRESULT GdsResTexture::LoadResource( const TCHAR* path , LPDIRECT3DDEVICE9 device )
{
	if ( device == NULL )
		return S_FALSE;

	if ( !_tcscmp( path , L"" ) )
		return S_FALSE;

	if ( SUCCEEDED( D3DXCreateTextureFromFile( device , path , &m_pTexture ) ) )
	{
		return S_OK;
	}

	return S_FALSE;
}