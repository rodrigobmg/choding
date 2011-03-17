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

	enum PROPERTY_TYPE{
		NONE_TYPE = -1,
		TEXTURE = 0,
		POLYGON,
	};

	GdsTexturePropertyPtr	GetTextureProperty();
	GdsPolygonPropertyPtr	GetPolygonProperty();

	void					Render( LPDIRECT3DDEVICE9 device );

private:
	
	LPDIRECT3DDEVICE9		m_pDevice;

	GdsTexturePropertyPtr	m_TextureProperty;
	GdsPolygonPropertyPtr	m_PolygonProperty;

};

typedef boost::shared_ptr< GdsPropertyState > GdsPropertyStatePtr;

#endif