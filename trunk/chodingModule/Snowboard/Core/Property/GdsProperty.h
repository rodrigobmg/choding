#ifndef _GDS_CORE_PROPERTY_H_
#define _GDS_CORE_PROPERTY_H_

#include "../Base/GdsObject.h"

class GdsProperty : public GdsObject
{
public:
	GdsProperty(void);
	virtual ~GdsProperty(void);

	enum PROPERTY_TYPE{
		NONE_TYPE = -1,
		TEXTURE = 0,
		POLYGON,
	};


	PROPERTY_TYPE		GetType(){ return m_eType; }	
	void				Clear(){ vClear(); }
	void				Update( float fElapsedTime ){ vUpdate( fElapsedTime ); }
	void				Render( LPDIRECT3DDEVICE9 device ){ vRender( device ); }


protected:

	void				SetType( PROPERTY_TYPE type ){ m_eType = type; }
	
	virtual	void		vClear(){};
	virtual	void		vUpdate( float fElapsedTime ){ ASSERT( fElapsedTime); }
	virtual void		vRender( LPDIRECT3DDEVICE9 device ){ ASSERT( device); }

	PROPERTY_TYPE		m_eType;
	LPDIRECT3DDEVICE9	m_pDevice;

};

typedef boost::shared_ptr< GdsProperty > GdsPropertyPtr;

#endif