#ifndef _GDS_CORE_PROPERTY_H_
#define _GDS_CORE_PROPERTY_H_

#include "../Base/GdsObject.h"

class GdsProperty : public GdsObject
{
public:
	GdsProperty(void);
	virtual ~GdsProperty(void);

	typedef boost::shared_ptr< GdsProperty > GdsPropertyPtr;

	enum PROPERTY_TYPE{
		NONE_TYPE = -1,
		TEXTURE = 0,
		VERTEX_BUFFER,
		INDEX_BUFFER,
	};

	void			Update( float fElapsedTime );
	void			SetType( PROPERTY_TYPE type );
	PROPERTY_TYPE	GetType(){ return m_eType; }	

	GdsPropertyPtr	GetProperty( PROPERTY_TYPE type );
	void			Clear(){ vClear(); }

private:

	GdsPropertyPtr	create_property( PROPERTY_TYPE type );

protected:
	
	PROPERTY_TYPE	m_eType;

 	typedef std::map< PROPERTY_TYPE , GdsPropertyPtr >	PROPERTY_MAP;
 	PROPERTY_MAP	m_PropertyContainer;

	virtual void	vUpdate( float fElapsedTime ) = 0;
	virtual void	vClear();
};

typedef boost::shared_ptr< GdsProperty > GdsPropertyPtr;

#endif