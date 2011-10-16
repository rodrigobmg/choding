#include "stdafx.h"

/////////////////////////////////////////////////////////////////////////////
// Initialize & Destroy Methods

// DS:: Create an instance of this class.
GdsSyncCriticalSection *GdsSyncCriticalSection::New()
{
	// Create it here.
	return new GdsSyncCriticalSection;
}

// DS:: Constructor.
GdsSyncCriticalSection::GdsSyncCriticalSection()
{
}

// DS:: Destructor.
GdsSyncCriticalSection::~GdsSyncCriticalSection()
{
	this->LeaveLock();
	::DeleteCriticalSection( &m_oCriticalSection );
}


/////////////////////////////////////////////////////////////////////////////
// Member functions.

// DS:: Create synchroinzition object.
bool GdsSyncCriticalSection::CreateSynchronize()
{
	::InitializeCriticalSection(&this->m_oCriticalSection);

	return true;
}

// DS:: Enter lock.
void GdsSyncCriticalSection::EnterLock()
{
	::EnterCriticalSection(&this->m_oCriticalSection);
}

// DS:: Leave lock.
void GdsSyncCriticalSection::LeaveLock()
{
	::LeaveCriticalSection(&this->m_oCriticalSection);
}