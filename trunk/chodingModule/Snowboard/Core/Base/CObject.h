#pragma once

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
			assert( "해제할때 0 이어야함" );
	}	

	void DecRefCount()		
	{ 
		--m_iRefCount; 
		if ( m_iRefCount < 0 )
			assert( "왜 0보다 작나요? " );
	}
	
	int32_t GetRefCount()		{ return m_iRefCount; }
	void IncRefCount()		{ ++m_iRefCount; }

private:

	void vInitialize()		{ m_iRefCount = 0; }


	int32_t m_iRefCount;
};

#endif