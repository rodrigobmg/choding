#include "Precompiled.h"
#include "sunMaterial.h"

ImplementBPool( sunMaterial );


sunMaterial::sunMaterial() :
m_dwDifuseColor(0)
{
	ZeroMemory(&m_MeshMtrl, sizeof(m_MeshMtrl));
	memset( m_spTextures, 0, sizeof(m_spTextures) );
}

sunMaterial::~sunMaterial()
{

}


void sunMaterial::SetTexture(sunTexturePtr spTexture, UINT uiStage)
{
	assert( uiStage < MAX_TEXTURE_STAGE );

	m_spTextures[uiStage] = spTexture;
}

sunTexturePtr sunMaterial::GetTexture( UINT uiStage )
{
	assert( uiStage < MAX_TEXTURE_STAGE );

	if( uiStage == BASETEX0 )
		return m_spTextures[uiStage];
	if( uiStage == NORMALMAP )
		return m_spTextures[uiStage];

	return NULL;
}

void sunMaterial::SetMaterialInfo(D3DMATERIAL9 kMaterial)
{
	m_MeshMtrl = kMaterial;
// 	m_MeshMtrl.Diffuse = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
// 	m_MeshMtrl.Ambient = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
// 	m_MeshMtrl.Specular = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
// 	m_MeshMtrl.Emissive = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
}

D3DMATERIAL9 sunMaterial::GetMaterial()
{

	return m_MeshMtrl;
}
