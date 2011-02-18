//
// "kSynchronize.h" : This class for defining sychronization object.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __SNOWBOARD_UTILITY_SYNC_H__
#define __SNOWBOARD_UTILITY_SYNC_H__

#include "../../Framework/Snowboard_stdafx.h"
#include "../../Core/Base/Snow.h"


/////////////////////////////////////////////////////////////////////////////
// class kSynchronize

class Synchronize : public CSnow
{
//
// Initialize & Destroy Methods
public:
	static Synchronize *New();

	Synchronize();
	virtual ~Synchronize(); 
	Synchronize(const Synchronize&) {} 
	void operator=(const Synchronize&) {} 


//
// Member Functions
public:

	// Create synchroinzition object.
	virtual bool CreateSynchronize()	{	return false;	};

	// Lock function.
	virtual void EnterLock()			{};
	virtual void LeaveLock()			{};

	// Syncronize object locking and unlocking.
	static void Lock(Synchronize *pSyncObj);
	static void UnLock(Synchronize *pSyncObj);
};


#endif	// __SNOWBOARD_UTILITY_SYNC_H__