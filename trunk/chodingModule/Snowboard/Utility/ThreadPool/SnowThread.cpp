#include "SnowThread.h"
#include "SyncCriticalSection.h"

/////////////////////////////////////////////////////////////////////////////
// Initialize & Destroy Methods

// DS:: Create an instance of this class.
SnowThread *SnowThread::New()
{
	// Create it here.
	return new SnowThread;
}

// DS:: Constructor.
SnowThread::SnowThread()
{
	SetName( OBJECT_THREAD );

	this->m_hThread = NULL;
	this->m_nThreadID = 0;
	this->m_bWorkDone = TRUE;
	this->m_bThreadExit = FALSE;
	this->m_nTimeOut = INFINITE;
	this->m_hEvent = NULL;
	this->m_bRunProcess = FALSE;

	this->m_nPriority = THREAD_PRIORITY_NORMAL;

	this->m_pThradFunc = NULL;
	this->m_pSyncObject = NULL;	
}

// DS:: Destructor.
SnowThread::~SnowThread()
{
	SAFE_DELETE(this->m_pSyncObject);
	this->ExitThread();

	if(this->m_hEvent)
	{
		::SetEvent(this->m_hEvent);
		::CloseHandle(this->m_hEvent);
		this->m_hEvent = NULL;
	}
}


/////////////////////////////////////////////////////////////////////////////
// Member functions.

// DS:: Create and run thread.
//		CreateEvent() -> CreateThread() -> ResumeThread().
BOOL SnowThread::CreateAndRunThread()
{
	// Create event.
	if(!this->CreateEvent())
		return FALSE;

	if(!this->CreateThread())
	{
		::SetEvent(this->m_hEvent);
		::CloseHandle(this->m_hEvent);
		this->m_hEvent = NULL;
		return FALSE;
	}

	if(!this->SetThreadPriority(this->m_nPriority))
	{
		this->ExitThread();

		::SetEvent(this->m_hEvent);
		::CloseHandle(this->m_hEvent);
		this->m_hEvent = NULL;
		return FALSE;
	}

	if(this->ResumeThread() == 0xFFFFFFFF)
	{
		this->ExitThread();

		::SetEvent(this->m_hEvent);
		::CloseHandle(this->m_hEvent);
		this->m_hEvent = NULL;
	}

	return TRUE;
}

// DS:: Starts execution of a SnowThread object.
BOOL SnowThread::CreateThread(DWORD dwCreateFlags, UINT nStackSize, 
							  LPSECURITY_ATTRIBUTES lpSecurityAttrs)
{
	// The ExitThread function ends a thread.
	this->ExitThread();

	this->InitInstance();

	if(!this->m_pThradFunc)
		this->SetThreadFunc();

	this->m_hThread = 
		::CreateThread(lpSecurityAttrs, nStackSize, 
		this->m_pThradFunc, (void *)this, 
		dwCreateFlags, &this->m_nThreadID);
	if(!this->m_hThread)
		return FALSE;

	return TRUE;
}

// DS:: The CreateEvent function creates a named or unnamed event object. 
BOOL SnowThread::CreateEvent(LPSECURITY_ATTRIBUTES lpEventAttributes,
							 BOOL bManualReset, BOOL bInitialState,
							 LPCTSTR lpName)
{
	if(this->m_hEvent)
	{
		::SetEvent(this->m_hEvent);
		::CloseHandle(this->m_hEvent);
	}

	this->m_hEvent = 
		::CreateEvent(lpEventAttributes, bManualReset, bInitialState, lpName);

	if(!this->m_hEvent)
		return FALSE;

	return TRUE;
}

// DS:: Destroy thread.
void SnowThread::DestroyThread()
{
	SAFE_DELETE(this->m_pSyncObject);
	this->ExitThread();

	if(this->m_hEvent)
	{
		::SetEvent(this->m_hEvent);
		::CloseHandle(this->m_hEvent);
		this->m_hEvent = NULL;
	}
}

// DS:: The ExitThread function ends a thread.
void SnowThread::ExitThread()
{
	if(!this->m_hThread)		return;

	this->m_bThreadExit = TRUE;
	if(this->m_hEvent)
		::SetEvent(this->m_hEvent);

#if 1
	DWORD	dwExitCode;
	while(::GetExitCodeThread(this->m_hThread, &dwExitCode) == TRUE &&
		dwExitCode == STILL_ACTIVE)
	{
		::WaitForSingleObject(this->m_hThread, 50);
	}  // polling code(bad)
#else

	DWORD	dwExitCode;
	if(::GetExitCodeThread(this->m_hThread, &dwExitCode))
	{
		::TerminateThread(this->m_hThread, dwExitCode);
	}
	char strMsg[512];
	::sprintf(strMsg, "SnowThread::ExitThread()-3 EXIT:%d - ERROR:%d", dwExitCode, ::GetLastError());
	kOutputMsg01(strMsg);

	::WaitForSingleObject(this->m_hThread, INFINITE);

	//kOutputMsg01("SnowThread::ExitThread()-4");

#endif
	::CloseHandle(this->m_hThread);
	this->m_hThread = NULL;
}

// DS:: Sets the priority of the current thread.
BOOL SnowThread::SetThreadPriority(int nPriority)
{
	if(!this->m_hThread)
		return false;

	// Setting thread priority
	this->m_nPriority = nPriority;

	return ::SetThreadPriority(this->m_hThread, nPriority);
}

// DS:: Decrements a thread¡¯s suspend count.
DWORD SnowThread::ResumeThread()
{
	if(!this->m_hThread)
		return 0xFFFFFFFF;

	return ::ResumeThread(this->m_hThread);
}

// DS:: Increments a thread¡¯s suspend count.
DWORD SnowThread::SuspendThread()
{
	if(!this->m_hThread)
		return 0xFFFFFFFF;

	return ::SuspendThread(this->m_hThread);
}

// DS:: Window process thread.
bool SnowThread::WinProcessThread()
{
	this->SetThreadDone(FALSE);
	for( ;; )
	{
		if(!this->ProcessThread())
			break;
	}
	this->SetThreadDone(TRUE);

	// Exit thread instance.
	this->ExitInstance();

	return true;
}

// DS:: Process thread.
bool SnowThread::ProcessThread()
{
	DWORD dwObject = WAIT_FAILED;
	if(this->m_hEvent)
		dwObject = ::WaitForSingleObject(this->m_hEvent, this->m_nTimeOut);

	if(this->GetThreadExit())		
		return false;

	switch(dwObject)
	{
	case WAIT_OBJECT_0 :
		{
			if(!this->GetRunProcess())
			{
				this->Run();
			}
		}
		break;
	case WAIT_TIMEOUT :
		break;
	}
	return true;
}

// DS:: Controlling function for threads with a message pump. 
//		Override to customize the default message loop.
void SnowThread::Run()
{
	// Will be overriden by subclass.
}

// Override to perform thread instance initialization.
BOOL SnowThread::InitInstance()
{
	// Create synchronization object.
	SAFE_DELETE(this->m_pSyncObject);
	this->m_pSyncObject = SyncCriticalSection::New();
	this->m_pSyncObject->CreateSynchronize();

	return TRUE;
}

// Override to clean up when your thread terminates.
void SnowThread::ExitInstance()
{
}

// DS:: Enter lock.
void SnowThread::EnterLock()
{
	if(this->m_pSyncObject)
		this->m_pSyncObject->EnterLock();
}

// DS:: Leave lock.
void SnowThread::LeaveLock()
{
	if(this->m_pSyncObject)
		this->m_pSyncObject->LeaveLock();
}

// DS:: Wating blocking event.
void SnowThread::WaitingBlockingEvent(HANDLE *pEvent, DWORD dwTimeOut)
{
	if(!pEvent)
		return;

	DWORD dwObject = WAIT_FAILED;
	for(;;)
	{
		if(!(*pEvent))
			break;

		dwObject = ::WaitForSingleObject(*pEvent, dwTimeOut);
		switch(dwObject)
		{
		case WAIT_OBJECT_0 :
			return;
		case WAIT_ABANDONED :
			//kOutputMsg01("SnowThread()...Abandoned...");
			return;
		case WAIT_TIMEOUT :
			//kOutputMsg01("SnowThread()...Time out...");
			break;
		default :
			//kOutputMsg01("SnowThread()...Unknown...");
			break;
		}
		::Sleep(1);
	}
}