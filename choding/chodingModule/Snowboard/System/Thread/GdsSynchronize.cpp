//
// "kSynchronize.cpp" : This class for defining sychronization object.
//
/////////////////////////////////////////////////////////////////////////////

#include "GdsSynchronize.h"


/////////////////////////////////////////////////////////////////////////////
// Initialize & Destroy Methods

// DS:: Create an instance of this class.
GdsSynchronize *GdsSynchronize::New()
{
	// Create it here.
	return new GdsSynchronize;
}

// DS:: Constructor.
GdsSynchronize::GdsSynchronize()
{
	SetName( OBJECT_SYNCCHRONIZE );
}

// DS:: Destructor.
GdsSynchronize::~GdsSynchronize()
{
}


/////////////////////////////////////////////////////////////////////////////
// Member functions.

// DS:: Syncronize object locking.
void GdsSynchronize::Lock(GdsSynchronize* pSyncObj)
{
	if(!pSyncObj)
		return;
	pSyncObj->EnterLock();
}

// DS:: Syncronize object unlocking.
void GdsSynchronize::UnLock(GdsSynchronize* pSyncObj)
{
	if(!pSyncObj)
		return;
	pSyncObj->LeaveLock();
}
