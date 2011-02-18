#ifndef __SNOWBOARD_UTILITY_SYNCCRITIALSECTION__
#define __SNOWBOARD_UTILITY_SYNCCRITIALSECTION__

#include "../../Framework/Snowboard_stdafx.h"

#include "Synchronize.h"


class SyncCriticalSection : public Synchronize
{
	//
	// Member Variables
protected:

	// Critical section object.
	CRITICAL_SECTION	m_oCriticalSection;
	//
	// Initialize & Destroy Methods
public:
	static SyncCriticalSection *New();

	SyncCriticalSection();
	virtual ~SyncCriticalSection(); 
	SyncCriticalSection(const SyncCriticalSection&) {} 
	void operator=(const SyncCriticalSection&) {} 


	//
	// Member Functions
public:

	// Create synchroinzition object.
	virtual bool CreateSynchronize();

	// Lock function.
	virtual void EnterLock();
	virtual void LeaveLock();

};



#endif