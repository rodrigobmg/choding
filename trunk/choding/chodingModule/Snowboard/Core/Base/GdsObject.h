#ifndef _SNOWBOARD_SNOW_OBJECT__
#define _SNOWBOARD_SNOW_OBJECT__

#include "../../Framework/Snowboard_stdafx.h"
#include "../../Framework/Snowboard_macro.h"

class GdsObject
{
public:
	GdsObject();
	virtual ~GdsObject();

	void			DecRefCount();
	void			IncRefCount()						{ InterlockedIncrement((LONG*)&m_iRefCount); /*++m_iRefCount;*/ }
	int32_t			GetRefCount()						{ return m_iRefCount; }	

	void			SetName(const tstring& strName )	{ m_strName = strName; }
	const tstring&	GetName()							{ return m_strName; }

private:

	int32_t		m_iRefCount;
	tstring		m_strName;


};


typedef boost::shared_ptr< GdsObject >	GdsObjectPtr;

#endif