#include "GdsObject.h"

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
		assert( 0 && "�����Ҷ� 0 �̾����" );
}

void GdsObject::DecRefCount()
{
	/*--m_iRefCount; */
	InterlockedDecrement((LONG*)&m_iRefCount);
	if ( m_iRefCount < 0 )
		assert( 0 && "�� 0���� �۳���? " );

}