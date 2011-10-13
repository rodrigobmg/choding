//
// "kSynchronize.h" : This class for defining sychronization object.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __SNOWBOARD_UTILITY_SYNC_H__
#define __SNOWBOARD_UTILITY_SYNC_H__

#include "../../Framework/Snowboard_stdafx.h"
#include "../../Core/Base/GdsObject.h"


/////////////////////////////////////////////////////////////////////////////
// class kSynchronize

class GdsSynchronize : public GdsObject
{
//
// Initialize & Destroy Methods
public:
	static GdsSynchronize *New();

	GdsSynchronize();
	virtual ~GdsSynchronize(); 
	GdsSynchronize(const GdsSynchronize&) {} 
	void operator=(const GdsSynchronize&) {} 


//
// Member Functions
public:

	// Create synchroinzition object.
	virtual bool CreateSynchronize()	{	return false;	};

	// Lock function.
	virtual void EnterLock()			{};
	virtual void LeaveLock()			{};

	// Syncronize object locking and unlocking.
	static void Lock(GdsSynchronize* pSyncObj);
	static void UnLock(GdsSynchronize* pSyncObj);
};

typedef boost::shared_ptr< GdsSynchronize >	GdsSynchronizePtr;

#endif	// __SNOWBOARD_UTILITY_SYNC_H__