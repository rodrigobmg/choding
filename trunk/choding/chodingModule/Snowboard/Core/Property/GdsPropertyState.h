#ifndef _GDS_CORE_PROPERTYSTATE_H_
#define _GDS_CORE_PROPERTYSTATE_H_

#include "../Base/GdsObject.h"
#include "GdsTextureProperty.h"
#include "GdsPolygonProperty.h"

class GdsPropertyState : public GdsObject
{
public:
	GdsPropertyState(void);
	virtual ~GdsPropertyState(void);

	GdsTexturePropertyPtr	GetTextureProperty(){ return m_TextureProperty; }
	void					SetTextureProperty( GdsTexturePropertyPtr tex ){ m_TextureProperty = tex; }

	GdsPolygonPropertyPtr	GetPolygonProperty(){ return m_PolygonProperty; }
	void					SetPolygonProperty( GdsPolygonPropertyPtr polygon ){ m_PolygonProperty = polygon; }

	void					Render( LPDIRECT3DDEVICE9 device );

private:

	GdsTexturePropertyPtr	m_TextureProperty;
	GdsPolygonPropertyPtr	m_PolygonProperty;

};

typedef boost::shared_ptr< GdsPropertyState > GdsPropertyStatePtr;

#endif