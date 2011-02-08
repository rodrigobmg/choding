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

	return S_OK;
}

HRESULT CResTexture::Load( const wchar_t* name , LPDIRECT3DDEVICE9 device )
{
	if ( name == L"" || device == NULL )
	{
		assert( 0 );
		return S_FALSE;
	}

	if ( SUCCEEDED( D3DXCreateTextureFromFile( device , name , &m_pTexture ) ) )
	{
		return S_OK;
	}

	return S_FALSE;	
}