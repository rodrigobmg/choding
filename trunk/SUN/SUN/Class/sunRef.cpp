#include "Precompiled.h"
#include "sunRef.h"


sunRef::sunRef()
{
	m_uiRefCount = 0;
}


sunRef::~sunRef()
{	
	assert( 0 == m_uiRefCount );
}

void sunRef::Destory()
{
	
}

void sunRef::IncRefCount()
{
	++m_uiRefCount;
}

void sunRef::DecRefCount()
{
	--m_uiRefCount;

	if( 0 == m_uiRefCount)
		delete this;
}

unsigned int sunRef::GetRefCount() const
{
	return m_uiRefCount;
}
