#include "Snow.h"

CSnow::CSnow()
{
	m_iRefCount = 0;
	SetName( OBJECT_BASE );
	IncRefCount();	
}

CSnow::~CSnow()
{	
	DecRefCount();
	if ( m_iRefCount != 0 )
		assert( 0 && "�����Ҷ� 0 �̾����" );
}

void CSnow::DecRefCount()
{
	/*--m_iRefCount; */
	InterlockedDecrement((LONG*)&m_iRefCount);
	if ( m_iRefCount < 0 )
		assert( 0 && "�� 0���� �۳���? " );
}