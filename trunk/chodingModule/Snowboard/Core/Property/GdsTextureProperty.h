#ifndef _GDS_CORE_PROPERTY_TEXTURE_H_
#define _GDS_CORE_PROPERTY_TEXTURE_H_

#include "GdsProperty.h"

class GdsTextureProperty : public GdsProperty
{
public:

	enum TEXTURE_TYPE{
		MULTI_TEXTURE,
		BUMP,
	};

	GdsTextureProperty(void);
	virtual ~GdsTextureProperty(void);

	LPDIRECT3DTEXTURE9		GetTexturePtr(){ return m_Tex; }
	LPDIRECT3DTEXTURE9*		GetTexture(){ return &m_Tex; }


	void			Render( LPDIRECT3DDEVICE9 device );
	void			Clear();

protected:

	LPDIRECT3DTEXTURE9		m_Tex;

};

typedef boost::shared_ptr< GdsTextureProperty > GdsTexturePropertyPtr;

#endif