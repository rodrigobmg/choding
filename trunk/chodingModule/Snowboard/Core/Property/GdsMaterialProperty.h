#ifndef _GDS_CORE_PROPERTY_MATERIAL_H_
#define _GDS_CORE_PROPERTY_MATERIAL_H_

#include "GdsProperty.h"
#include "GdsTextureProperty.h"

class GdsMaterialProperty : public GdsProperty
{
	D3DMATERIAL9	m_dxMaterial;

	bool			m_bUseTexture;
	bool			m_bUseOpacity;

	GdsTexturePropertyPtr	m_TextureProperty;

public:
	GdsMaterialProperty();
	~GdsMaterialProperty();

	void			SetTexture( GdsTexturePropertyPtr ptex );

	void			SetAmbientColor( float r , float g , float b );
	void			SetDiffuesColor( float r , float g , float b );
	void			SetSpecularColor( float r , float g , float b );

};


#endif