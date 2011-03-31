#ifndef _GDS_CORE_PROPERTY_MATERIAL_H_
#define _GDS_CORE_PROPERTY_MATERIAL_H_

#include "GdsProperty.h"

/*
struct Material
{
	bool bUseTexture; // �ؽ��� �������
	bool bUseOpacity; // Opacity map �������
	char TextureName[256];
	int SubMaterialCount;
	Material* SubMaterial;
	LPDIRECT3DTEXTURE8 pTexture; // �ؽ���
	D3DMATERIAL8 d3dMaterial; // d3d ���� ��ü
	CTextureContainer* m_pTextureContainer; // �ؽ��� ���� class

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