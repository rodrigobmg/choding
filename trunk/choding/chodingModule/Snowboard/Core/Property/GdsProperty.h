#ifndef _GDS_CORE_PROPERTY_H_
#define _GDS_CORE_PROPERTY_H_

#include "../Base/GdsObject.h"

class GdsProperty : public GdsObject
{
public:
	GdsProperty(void)
	{
		SetName(OBJECT_PROPERTY); 
		SetType( NONE_TYPE ); 
	}

	virtual ~GdsProperty(void)
	{
	}

	enum PROPERTY_TYPE{
		NONE_TYPE = -1,
		TEXTURE = 0,
		POLYGON,
	};


	PROPERTY_TYPE	GetType(){ return m_eType; }	

	virtual void	Clear() = 0;


protected:

	void				SetType( PROPERTY_TYPE type ){ m_eType = type; }
	PROPERTY_TYPE		m_eType;

	LPDIRECT3DDEVICE9	m_pDevice;

};

typedef boost::shared_ptr< GdsProperty > GdsPropertyPtr;

#endif