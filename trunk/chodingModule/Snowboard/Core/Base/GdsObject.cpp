#include "GdsObject.h"

//ImplementBoostPool( GdsObject )

GdsObject::GdsObject()
{
	m_iRefCount = 0;
	SetName( OBJECT_BASE );
	IncRefCount();	
}

GdsObject::~GdsObject()
{	
	DecRefCount();
	if ( m_iRefCount != 0 )
		ASSERT( 0 && L"해제할때 0 이어야함" );
}

void GdsObject::DecRefCount()
{
	/*--m_iRefCount; */
	InterlockedDecrement((LONG*)&m_iRefCount);
	if ( m_iRefCount < 0 )
		ASSERT( 0 && L"왜 0보다 작나요? " );

}