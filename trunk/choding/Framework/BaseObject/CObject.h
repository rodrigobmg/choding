#ifndef _CHODING_BASEOBJECT_
#define _CHODING_BASEOBJECT_

#include "choding_stdafx.h"

class CObject
{
public:
	CObject()
	{
		vInitialize();
		IncRefCount();
	}
	virtual ~CObject()
	{
		DecRefCount();
		if ( m_iRefCount != 0 )
			assert( 0 && "해제할때 0 이어야함" );
	}	

	void DecRefCount()		
	{ 
		--m_iRefCount; 
		if ( m_iRefCount < 0 )
			assert( 0 && "왜 0보다 작나요? " );
	}
	
	int32_t GetRefCount()		{ return m_iRefCount; }
	void IncRefCount()		{ ++m_iRefCount; }

private:

	void vInitialize()		{ m_iRefCount = 0; }


	int32_t m_iRefCount;

};

#endif