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
		ASSERT( 0 && L"�����Ҷ� 0 �̾����" );
}

void GdsObject::DecRefCount()
{
	/*--m_iRefCount; */
	InterlockedDecrement((LONG*)&m_iRefCount);
	if ( m_iRefCount < 0 )
		ASSERT( 0 && L"�� 0���� �۳���? " );

}