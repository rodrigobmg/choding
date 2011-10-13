//
// "kSynchronize.cpp" : This class for defining sychronization object.
//
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

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
