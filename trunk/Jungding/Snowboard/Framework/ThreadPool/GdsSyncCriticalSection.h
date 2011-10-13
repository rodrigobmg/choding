#ifndef _SNOWBOARD_FRAMEWORK_SYSTEM_THREAD_SYSNC_CRI_H_
#define _SNOWBOARD_FRAMEWORK_SYSTEM_THREAD_SYSNC_CRI_H_

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