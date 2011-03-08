#ifndef __SNOWBOARD_UTILITY_GDSTHREAD__
#define __SNOWBOARD_UTILITY_GDSTHREAD__

#include "../../Framework/Snowboard_stdafx.h"
#include "../../Core/Base/GdsObject.h"
#include "GdsSynchronize.h"

class GdsThread : public GdsObject
{	
protected:

	HANDLE				m_hThread;
	DWORD				m_nThreadID;
	DWORD				m_nTimeOut;
	HANDLE				m_hEvent;
	BOOL				m_bWorkDone;
	BOOL				m_bThreadExit;
	BOOL				m_bRunProcess;
	int					m_nPriority;
	
	LPTHREAD_START_ROUTINE	m_pThradFunc;

	GdsSynchronize*		m_pSyncObject;

public:
	static GdsThread* New();

	GdsThread();
	virtual ~GdsThread(); 

public:

	static int WinThreadFunc(void *pParam)					
	{				
		GdsThread *pWinThread = (GdsThread *)pParam;					
		if(!pWinThread)													
			return -1;													
		pWinThread->WinProcessThread();								
			return 0;													
	}

	virtual void SetThreadFunc()		{ this->m_pThradFunc = (LPTHREAD_START_ROUTINE)WinThreadFunc; }

	void SetThreadID( DWORD ID )		{ m_nThreadID = ID ;}
	DWORD GetThreadId()					{ return m_nThreadID; }

	void SetTimeOut( DWORD val )		{ m_nTimeOut = val; }
	DWORD GetTimeOut()					{ return m_nTimeOut; }

	void SetThreadDone( BOOL flag )		{ m_bWorkDone = flag; }
	BOOL GetThreadDone()				{ return m_bWorkDone; }

	void SetThreadExit( BOOL flag )		{ m_bThreadExit = flag; }
	BOOL GetThreadExit()				{ return m_bThreadExit; }
	
	void SetRunProcess( BOOL flag )		{ m_bRunProcess = flag; }
	BOOL GetRunProcess()				{ return m_bRunProcess; }
	
	void SetPriority( int iPriority )	{ m_nPriority = iPriority; }
	int GetPriority()					{ return m_nPriority; }

	HANDLE GetThreadHandle()
	{
		return this->m_hThread;
	}

	HANDLE GetEventHandle()
	{
		return this->m_hEvent;
	}

	BOOL SetThreadPriority(int nPriority);

	DWORD ResumeThread();
	DWORD SuspendThread();

	virtual BOOL CreateEvent(LPSECURITY_ATTRIBUTES lpEventAttributes = NULL,
		BOOL bManualReset = FALSE, BOOL bInitialState = FALSE,
		LPCTSTR lpName = NULL);

	virtual BOOL CreateThread(DWORD dwCreateFlags=CREATE_SUSPENDED, UINT nStackSize=0, 
		LPSECURITY_ATTRIBUTES lpSecurityAttrs=NULL);

	virtual void DestroyThread();
	bool WinProcessThread();

protected:

	void ExitThread();

	virtual void EnterLock();
	virtual void LeaveLock();

	void WaitingBlockingEvent(HANDLE *hEvent, DWORD dwTimeOut);
	virtual bool ProcessThread();

public:
	virtual BOOL CreateAndRunThread();

	// 이함수를 오버라이딩해서 쓴다.
	virtual void Run();

	virtual BOOL InitInstance();
	virtual void ExitInstance();
};

typedef boost::shared_ptr< GdsThread >	GdsThreadPtr;

#endif