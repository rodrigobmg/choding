#ifndef _GDS_CORE_PROPERTY_MATERIAL_H_
#define _GDS_CORE_PROPERTY_MATERIAL_H_

#include "GdsProperty.h"
#include "GdsTextureProperty.h"

class GdsMaterialProperty : public GdsProperty
{
public:

	enum TEXTURE_TYPE{
		DEFAULT,
		MULTI_TEXTURE,
		BUMP,
	};

	enum MULTI_TEXTURE_MAX{
		MAX = 4,
	};

private:

	D3DMATERIAL9	m_dxMaterial;

	bool			m_bUseTexture;
	bool			m_bUseOpacity;

	TEXTURE_TYPE						m_eTexturingType;
	int32_t								m_iCountOfTexture;
	typedef std::vector< LPDIRECT3DTEXTURE9	>	TEXTURE_CONTAINER;
	TEXTURE_CONTAINER	m_Tex;


public:

	GdsMaterialProperty();
	~GdsMaterialProperty();

	void			SetTexture( GdsTexturePropertyPtr ptex );

	void			SetAmbientColor( float r , float g , float b );
	void			SetDiffuesColor( float r , float g , float b );
	void			SetSpecularColor( float r , float g , float b );


	LPDIRECT3DTEXTURE9		GetTexturePtr( const int num = 0 );
	LPDIRECT3DTEXTURE9*		GetTexture( const int num = 0 );

	void			SetTexture( LPDIRECT3DTEXTURE9 texture , const int num = 0 );


	void			Render( LPDIRECT3DDEVICE9 device );
	void			Clear();

};

typedef boost::shared_ptr< GdsMaterialProperty > GdsMaterialPropertyPtr;

#endif