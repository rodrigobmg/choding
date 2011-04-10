#include "Precompiled.h"
#include "sunTexture.h"


ImplementBPool(sunTexture);


sunTexture::sunTexture():
m_pTexture(NULL),
m_pSurface(NULL)
{

}

sunTexture::~sunTexture()
{
	Safe_Release(m_pTexture);
	Safe_Release(m_pSurface);
}

// bool sunTexture::Load()
// {
// 	sunRenderer* pRenderer =  static_cast<sunGraphicCore*> (g_pCoreManager->GetCore(CORENAME_GRAPHIC) )->GetRenderer();
// 	assert(pRenderer);
// 
// 	if(pRenderer->GetRendererID() == RENDERER_DX9)
// 	{
// 		sunRendererDX9* pRenderDX9 = static_cast<sunRendererDX9*>(pRenderer);
// 
// 		if( FAILED( D3DXCreateTextureFromFile( pRenderDX9->GetDevice(), m_strFileName.c_str(), &m_pTexture ) ) )
// 		{
// 			return false;
// 		}
// 
// 		return true;
// 	}
// 
// 	return false;
// }

bool sunTexture::Load(const tstring& strFileName)
{
	sunRenderer* pRenderer =  static_cast<sunGraphicCore*> (g_pCoreManager->GetCore(CORENAME_GRAPHIC) )->GetRenderer();
	assert(pRenderer);

	if( strFileName.length() <= 0)
		assert(0 && "strFileName length <= 0");


	tstring FullPath = L"Resource\\" + strFileName;

	if(pRenderer->GetRendererID() == RENDERER_DX9)
	{
		sunRendererDX9* pRenderDX9 = static_cast<sunRendererDX9*>(pRenderer);

		if( FAILED( D3DXCreateTextureFromFile( pRenderDX9->GetDevice(), FullPath.c_str(), &m_pTexture ) ) )
		{
			assert(0 && "CreateTexture Failed");
			return false;
		}

		return true;
	}

	return false;
}

LPDIRECT3DTEXTURE9 sunTexture::GetDX9Texture()
{
	assert(m_pTexture);
	return m_pTexture;
}

LPDIRECT3DSURFACE9 sunTexture::GetDX9Surface( int iLevel )
{
	if( FAILED( m_pTexture->GetSurfaceLevel( iLevel, &m_pSurface ) ) )
	{
		return false;
	}
	assert(m_pSurface);

	return m_pSurface;
}