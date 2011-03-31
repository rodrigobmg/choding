#ifndef _GDS_CORE_PROPERTY_MATERIAL_H_
#define _GDS_CORE_PROPERTY_MATERIAL_H_

#include "GdsProperty.h"

/*
struct Material
{
	bool bUseTexture; // 텍스춰 사용유무
	bool bUseOpacity; // Opacity map 사용유무
	char TextureName[256];
	int SubMaterialCount;
	Material* SubMaterial;
	LPDIRECT3DTEXTURE8 pTexture; // 텍스쳐
	D3DMATERIAL8 d3dMaterial; // d3d 재질 객체
	CTextureContainer* m_pTextureContainer; // 텍스쳐 관리 class

	Material():
	bUseTexture(false),
		bUseOpacity(false),
		SubMaterialCount(0),
		SubMaterial(NULL),
		pTexture(NULL),
		m_pTextureContainer(NULL)
	{
	}
	~Material()
	{
		if(SubMaterial)
			delete[] SubMaterial;
		if(m_pTextureContainer&&pTexture)
			m_pTextureContainer->Delete(pTexture);
	}
};*/


class GdsMaterialProperty : public GdsProperty
{
	D3DMATERIAL9		m_Material;
};


#endif