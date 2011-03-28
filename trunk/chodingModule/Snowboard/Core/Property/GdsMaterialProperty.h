#ifndef _GDS_CORE_PROPERTY_TEXTURE_H_
#define _GDS_CORE_PROPERTY_TEXTURE_H_

#include "GdsProperty.h"

class GdsTextureProperty : public GdsProperty
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

	GdsTextureProperty(void);
	virtual ~GdsTextureProperty(void);

	LPDIRECT3DTEXTURE9		GetTexturePtr( const int num = 0 );
	LPDIRECT3DTEXTURE9*		GetTexture( const int num = 0 );

	void			SetTexture( LPDIRECT3DTEXTURE9 texture , const int num = 0 );


	void			Render( LPDIRECT3DDEVICE9 device );
	void			Clear();

private:

	TEXTURE_TYPE						m_eTexturingType;
	int32_t								m_iCountOfTexture;
	std::vector< LPDIRECT3DTEXTURE9	>	m_Tex;

};

typedef boost::shared_ptr< GdsTextureProperty > GdsTexturePropertyPtr;

#endif