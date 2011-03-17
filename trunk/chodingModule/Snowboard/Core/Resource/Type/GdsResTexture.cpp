#include "GdsResTexture.h"

ImplementBoostPool( GdsResTexture )

GdsResTexture::GdsResTexture()
{
	SetName( OBJECT_RES_TEXTURE );
	vClear();
}

GdsResTexture::~GdsResTexture()
{
	vClear();
}

void GdsResTexture::vClear()
{
}

HRESULT GdsResTexture::vCreate( const TCHAR* path , LPDIRECT3DDEVICE9 device )
{

	if ( device == NULL )
		return false;

	m_strPath = path;
	if ( vLoadResource() )
	{

	}
	return true;
}

HRESULT GdsResTexture::vRelease()
{	
	return true;
}

HRESULT GdsResTexture::vLoadResource( LPDIRECT3DDEVICE9 device )
{
	if ( m_strPath == L"" )
		return false;

	GdsTexturePropertyPtr texture = m_PropertyState->GetTextureProperty();
	if ( SUCCEEDED( D3DXCreateTextureFromFile( device , path , texture->GetTexture() ) ) )
	{
		return true;
	}

	return false;
}

HRESULT GdsResTexture::vReCreate( LPDIRECT3DDEVICE9 device )
{
	return vLoadResource( device );
}
