#ifndef __SNOWBOARD_UTILITY_SYNCCRITIALSECTION__
#define __SNOWBOARD_UTILITY_SYNCCRITIALSECTION__

#include "../../Framework/Snowboard_stdafx.h"

#include "GdsSynchronize.h"


class GdsSyncCriticalSection : public GdsSynchronize
{
	//
	// Member Variables
protected:

	// Critical section object.
	CRITICAL_SECTION	m_oCriticalSection;
	//
	// Initialize & Destroy Methods
public:
	static GdsSyncCriticalSection *New();

	GdsSyncCriticalSection();
	virtual ~GdsSyncCriticalSection(); 
	GdsSyncCriticalSection(const GdsSyncCriticalSection&) {} 
	void operator=(const GdsSyncCriticalSection&) {} 


	//
	// Member Functions
public:

	// Create synchroinzition object.
	virtual bool CreateSynchronize();

	// Lock function.
	virtual void EnterLock();
	virtual void LeaveLock();

};

typedef boost::shared_ptr< GdsSyncCriticalSection > GdsSyncCriticalSectionPtr;

#endif