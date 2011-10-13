//
// "kSynchronize.h" : This class for defining sychronization object.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _SNOWBOARD_FRAMEWORK_SYSTEM_THREAD_SYSNCHRONIZE_H_
#define _SNOWBOARD_FRAMEWORK_SYSTEM_THREAD_SYSNCHRONIZE_H_

/////////////////////////////////////////////////////////////////////////////
// class kSynchronize

class GdsSynchronize
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