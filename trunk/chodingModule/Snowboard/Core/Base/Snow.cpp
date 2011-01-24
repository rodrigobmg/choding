#include "Snow.h"

CSnow::CSnow()
{
	m_iRefCount = 0;
	m_strName = L"Base";

	IncRefCount();	
}

CSnow::~CSnow()
{
	DecRefCount();
	if ( m_iRefCount != 0 )
		assert( "해제할때 0 이어야함" );
}

void CSnow::DecRefCount()
{
	--m_iRefCount; 
	if ( m_iRefCount < 0 )
		assert( "왜 0보다 작나요? " );
}