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
			assert( "�����Ҷ� 0 �̾����" );
	}	

	void DecRefCount()		
	{ 
		--m_iRefCount; 
		if ( m_iRefCount < 0 )
			assert( "�� 0���� �۳���? " );
	}
	
	int32_t GetRefCount()		{ return m_iRefCount; }
	void IncRefCount()		{ ++m_iRefCount; }

private:

	void vInitialize()		{ m_iRefCount = 0; }


	int32_t m_iRefCount;
};

#endif