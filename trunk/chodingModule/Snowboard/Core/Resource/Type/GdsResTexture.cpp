#include "GdsResTexture.h"

//ImplementBoostPool( GdsResTexture )

GdsResTexture::GdsResTexture():
m_pTexture( NULL )
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
	SAFE_RELEASE( m_pTexture );
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

	if ( SUCCEEDED( D3DXCreateTextureFromFileEx( device , m_strPath.c_str() , D3DX_DEFAULT , D3DX_DEFAULT , D3DX_DEFAULT , 0 ,
								D3DFMT_X8R8G8B8 ,
								D3DPOOL_MANAGED , 
								D3DX_DEFAULT , D3DX_DEFAULT , 0 ,
								NULL , NULL , 
								&m_pTexture
								) ) )
	{
		return true;
	}

	return false;
}
