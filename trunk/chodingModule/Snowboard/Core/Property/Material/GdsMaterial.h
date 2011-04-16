#ifndef _GDS_CORE_PROPERTY_MATERIAL_H_
#define _GDS_CORE_PROPERTY_MATERIAL_H_

#include "../Base/GdsBaseProperty.h"
#include "../../Resource/Type/GdsResTexture.h"

class GdsMaterial : public GdsBaseProperty
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

	D3DMATERIAL9								m_dxMaterial;

	TEXTURE_TYPE								m_eTexturingType;
	int32_t										m_iCountOfTexture;
	typedef std::vector< GdsResTexturePtr >		TEXTURE_CONTAINER;
	TEXTURE_CONTAINER							m_Tex;

	typedef std::vector< tstring >				TEXTURE_PATH;
	TEXTURE_PATH								m_TexturePath;

protected:

	virtual void			vClear();

public:

	GdsMaterial();
	virtual ~GdsMaterial();


	void					SetAmbientColor( float r , float g , float b );
	void					SetDiffuesColor( float r , float g , float b );
	void					SetSpecularColor( float r , float g , float b );

	GdsResTexturePtr		GetTexture( const int num = 0 );
	void					SetTexture( GdsResTexturePtr texture , const int num = 0 );

	void					SetTexturePath( tstring& path , const int index = 0 );
	tstring&				GetTexturePath( const int index = 0 );


	void					Render( LPDIRECT3DDEVICE9 device );

};

typedef boost::shared_ptr< GdsMaterial > GdsMaterialPtr;

#endif