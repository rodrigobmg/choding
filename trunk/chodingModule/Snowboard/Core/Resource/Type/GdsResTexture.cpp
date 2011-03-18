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

	GdsTexturePropertyPtr texture = m_PropertyState->GetTextureProperty();
// 	if ( SUCCEEDED( D3DXCreateTextureFromFile( device , m_strPath.c_str() , texture->GetTexture() ) ) )
// 	{
// 		return true;
// 	}
	if ( SUCCEEDED( D3DXCreateTextureFromFileEx( device , m_strPath.c_str() , D3DX_DEFAULT , D3DX_DEFAULT , D3DX_DEFAULT , 0 ,
								D3DFMT_X8R8G8B8 ,
								D3DPOOL_MANAGED , 
								D3DX_DEFAULT , D3DX_DEFAULT , 0 ,
								NULL , NULL , texture->GetTexture() ) ) )
	{
		return true;
	}

	return false;
}
