#include "GdsResTexture.h"

ImplementBoostPool( GdsResTexture )

GdsResTexture::GdsResTexture()
{
	SetName( OBJECT_RES_TEXTURE );
	vClear();
}

GdsResTexture::~GdsResTexture()
{
	if ( m_pTexture )
		m_pTexture->Release();	
	
	vClear();
}

void GdsResTexture::vClear()
{
	m_pTexture = NULL;
}

HRESULT GdsResTexture::vCreate()
{
	return true;
}

HRESULT GdsResTexture::vRelease()
{	
	return true;
}

HRESULT GdsResTexture::vLoadResource( const TCHAR* path , LPDIRECT3DDEVICE9 device )
{
	if ( device == NULL )
		return false;

	if ( !_tcscmp( path , L"" ) )
		return false;

	if ( SUCCEEDED( D3DXCreateTextureFromFile( device , path , &m_pTexture ) ) )
	{
		m_strPath = path;
		return true;
	}

	return false;
}

HRESULT GdsResTexture::vReCreate( LPDIRECT3DDEVICE9 device )
{
	if ( m_pTexture )
		m_pTexture->Release();

	return vLoadResource( m_strPath.c_str() , device );
}
