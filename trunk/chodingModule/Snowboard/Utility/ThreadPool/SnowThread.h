#ifndef __SNOWBOARD_UTILITY_SNOWTHREAD__
#define __SNOWBOARD_UTILITY_SNOWTHREAD__

#include "../../Framework/Snowboard_stdafx.h"
#include "../../Core/Base/Snow.h"
#include "Synchronize.h"

class SnowThread : public CSnow
{	
protected:
	// Handle to the current thread.
	HANDLE				m_hThread;
	// ID of the current thread.
	DWORD				m_nThreadID;
	// Time-out interval in milliseconds.
	DWORD				m_nTimeOut;

	// Thread event.
	HANDLE				m_hEvent;

	// Done Thread working flag.
	BOOL				m_bWorkDone;
	BOOL				m_bThreadExit;

	// Running process flag.
	BOOL				m_bRunProcess;

	// Thread priority.
	int					m_nPriority;

	// Thread function.
	LPTHREAD_START_ROUTINE	m_pThradFunc;
	// Thread synchronization object.
	Synchronize*		m_pSyncObject;

	//
	// Initialize & Destroy Methods
public:
	static SnowThread* New();

	SnowThread();
	virtual ~SnowThread(); 

	//
	// Member Functions
public:

	static int WinThreadFunc(void *pParam)					
	{				
		SnowThread *pWinThread = (SnowThread *)pParam;					
		if(!pWinThread)													
			return -1;													
			pWinThread->WinProcessThread();								
			return 0;													
	}

	virtual void SetThreadFunc()		{ this->m_pThradFunc = (LPTHREAD_START_ROUTINE)WinThreadFunc; }

	// Get/Set thread id.
	void SetThreadID( DWORD ID )		{ m_nThreadID = ID ;}
	DWORD GetThreadId()					{ return m_nThreadID; }

	// Get/Set time-out interval in milliseconds.
	void SetTimeOut( DWORD val )		{ m_nTimeOut = val; }
	DWORD GetTimeOut()					{ return m_nTimeOut; }
	

	// Get/Set done Thread working flag.
	void SetThreadDone( BOOL flag )		{ m_bWorkDone = flag; }
	BOOL GetThreadDone()				{ return m_bWorkDone; }

	// Get/Set exit thread.
	void SetThreadExit( BOOL flag )		{ m_bThreadExit = flag; }
	BOOL GetThreadExit()				{ return m_bThreadExit; }
	
	// Get/Set run process.
	void SetRunProcess( BOOL flag )		{ m_bRunProcess = flag; }
	BOOL GetRunProcess()				{ return m_bRunProcess; }
	
	// Get/Set thread priority.
	void SetPriority( int iPriority )	{ m_nPriority = iPriority; }
	int GetPriority()					{ return m_nPriority; }

	// Get thread handle.
	HANDLE GetThreadHandle()
	{
		return this->m_hThread;
	}
	// Get thread event handle.
	HANDLE GetEventHandle()
	{
		return this->m_hEvent;
	}

	// Sets the priority of the current thread.
	BOOL SetThreadPriority(int nPriority);

	// Decrements a thread¡¯s suspend count.
	DWORD ResumeThread();
	// Increments a thread¡¯s suspend count.
	DWORD SuspendThread();

	// The CreateEvent function creates a named or unnamed event object. 
	virtual BOOL CreateEvent(LPSECURITY_ATTRIBUTES lpEventAttributes = NULL,
		BOOL bManualReset = FALSE, BOOL bInitialState = FALSE,
		LPCTSTR lpName = NULL);

	// Starts execution of a SnowThread object.
	virtual BOOL CreateThread(DWORD dwCreateFlags=CREATE_SUSPENDED, UINT nStackSize=0, 
		LPSECURITY_ATTRIBUTES lpSecurityAttrs=NULL);

	// Destroy thread.
	virtual void DestroyThread();

	// Process thread.
	bool WinProcessThread();

protected:
	// The ExitThread function ends a thread.
	void ExitThread();

	// Lock function.
	virtual void EnterLock();
	virtual void LeaveLock();

	// Wating blocking event.
	void WaitingBlockingEvent(HANDLE *hEvent, DWORD dwTimeOut);

	// Process thread.
	virtual bool ProcessThread();

	// Virtual functions.
public:
	// Create and run thread.
	// CreateEvent() -> CreateThread() -> ResumeThread().
	virtual BOOL CreateAndRunThread();

	// Controlling function for threads with a message pump. 
	// Override to customize the default message loop.
	virtual void Run();

	// Override to perform thread instance initialization.
	virtual BOOL InitInstance();

	// Override to clean up when your thread terminates.
	virtual void ExitInstance();

};


#endif