#ifndef _SUN_RESOURCE_SUNMATERIAL_H_
#define _SUN_RESOURCE_SUNMATERIAL_H_


//마테리얼 정보
class DLL_API sunMaterial : public sunRef
{
	DeclareBPool


public:
	typedef enum _eTextureStage
	{
		BASETEX0 = 0,
		BASETEX1,
		NORMALMAP,
		MAX_TEXTURE_STAGE,
	} eTextureStage;


public:
	sunMaterial();
	virtual ~sunMaterial();

public:
	void SetTexture( sunTexturePtr spTexture, UINT uiStage);
	void SetTexture( const tstring& strFileName, UINT uiStage);

	sunTexturePtr GetTexture( UINT uiStage );

	D3DMATERIAL9  GetMaterial();

public:
	void SetMaterialInfo( D3DMATERIAL9 kMaterial);

public:
	D3DMATERIAL9	m_MeshMtrl;

	DWORD	m_dwDifuseColor;
	

	sunTexturePtr	m_spTextures[MAX_TEXTURE_STAGE];

};













#endif