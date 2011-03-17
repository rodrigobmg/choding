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
		VERTEX_BUFFER,
		INDEX_BUFFER,
	};

	void			Update( float fElapsedTime );
	void			SetType( PROPERTY_TYPE type );
	PROPERTY_TYPE	GetType(){ return m_eType; }
	
protected:
	
	PROPERTY_TYPE	m_eType;

	virtual void	vUpdate( float fElapsedTime ) = 0;
};

#endif