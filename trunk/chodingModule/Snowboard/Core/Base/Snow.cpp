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
		assert( "�����Ҷ� 0 �̾����" );
}

void CSnow::DecRefCount()
{
	--m_iRefCount; 
	if ( m_iRefCount < 0 )
		assert( "�� 0���� �۳���? " );
}