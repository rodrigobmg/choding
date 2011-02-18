//
// "kSynchronize.cpp" : This class for defining sychronization object.
//
/////////////////////////////////////////////////////////////////////////////

#include "Synchronize.h"


/////////////////////////////////////////////////////////////////////////////
// Initialize & Destroy Methods

// DS:: Create an instance of this class.
Synchronize *Synchronize::New()
{
	// Create it here.
	return new Synchronize;
}

// DS:: Constructor.
Synchronize::Synchronize()
{
	SetName( OBJECT_SYNCCHRONIZE );
}

// DS:: Destructor.
Synchronize::~Synchronize()
{
}


/////////////////////////////////////////////////////////////////////////////
// Member functions.

// DS:: Syncronize object locking.
void Synchronize::Lock(Synchronize *pSyncObj)
{
	if(!pSyncObj)
		return;
	pSyncObj->EnterLock();
}

// DS:: Syncronize object unlocking.
void Synchronize::UnLock(Synchronize *pSyncObj)
{
	if(!pSyncObj)
		return;
	pSyncObj->LeaveLock();
}
