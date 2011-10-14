#include "GdsResTexture.h"

//ImplementBoostPool( GdsResTexture )

GdsResTexture::GdsResTexture():
m_TextureProperty( GdsTexturePropertyPtr( new GdsTextureProperty ) )
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

HRESULT GdsResTexture::vRelease()
{	
	return true;
}

HRESULT GdsResTexture::vLoadResource( LPDIRECT3DDEVICE9 device )
{
	if ( device == NULL )
	{
		ASSERT( 0 );
		return false;
	}

	if ( m_strPath == L"" )
	{
		ASSERT( 0 );
		return false;
	}

// 	if ( SUCCEEDED( D3DXCreateTextureFromFile( device , m_strPath.c_str() , texture->GetTexture() ) ) )
// 	{
// 		return true;
// 	}

	if ( SUCCEEDED( D3DXCreateTextureFromFileEx( device , m_strPath.c_str() , D3DX_DEFAULT , D3DX_DEFAULT , D3DX_DEFAULT , 0 ,
								D3DFMT_X8R8G8B8 ,
								D3DPOOL_MANAGED , 
								D3DX_DEFAULT , D3DX_DEFAULT , 0 ,
								NULL , NULL , 
								m_TextureProperty->GetTexture() //&m_pTex 
								) ) )
	{
		GetPropertyState()->SetTextureProperty( m_TextureProperty );
		return true;
	}

	return false;
}