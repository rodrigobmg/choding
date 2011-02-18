#include "SyncCriticalSection.h"


/////////////////////////////////////////////////////////////////////////////
// Initialize & Destroy Methods

// DS:: Create an instance of this class.
SyncCriticalSection *SyncCriticalSection::New()
{
	// Create it here.
	return new SyncCriticalSection;
}

// DS:: Constructor.
SyncCriticalSection::SyncCriticalSection()
{
	SetName( OBJECT_CRITICALSECTION );
}

// DS:: Destructor.
SyncCriticalSection::~SyncCriticalSection()
{
	this->LeaveLock();
}


/////////////////////////////////////////////////////////////////////////////
// Member functions.

// DS:: Create synchroinzition object.
bool SyncCriticalSection::CreateSynchronize()
{
	::InitializeCriticalSection(&this->m_oCriticalSection);

	return true;
}

// DS:: Enter lock.
void SyncCriticalSection::EnterLock()
{
	::EnterCriticalSection(&this->m_oCriticalSection);
}

// DS:: Leave lock.
void SyncCriticalSection::LeaveLock()
{
	::LeaveCriticalSection(&this->m_oCriticalSection);
}