#include "GdsThread.h"
#include "GdsSyncCriticalSection.h"

/////////////////////////////////////////////////////////////////////////////
// Initialize & Destroy Methods

// DS:: Create an instance of this class.
GdsThread *GdsThread::New()
{
	// Create it here.
	return new GdsThread;
}

// DS:: Constructor.
GdsThread::GdsThread()
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
GdsThread::~GdsThread()
{
	DestroyThread();
}


/////////////////////////////////////////////////////////////////////////////
// Member functions.

// DS:: Create and run thread.
//		CreateEvent() -> CreateThread() -> ResumeThread().
BOOL GdsThread::CreateAndRunThread()
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

// DS:: Starts execution of a GdsThread object.
BOOL GdsThread::CreateThread(DWORD dwCreateFlags, UINT nStackSize, 
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
BOOL GdsThread::CreateEvent(LPSECURITY_ATTRIBUTES lpEventAttributes,
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
void GdsThread::DestroyThread()
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
void GdsThread::ExitThread()
{
	if(!this->m_hThread)		return;

	this->m_bThreadExit = TRUE;
	if(this->m_hEvent)
		::SetEvent(this->m_hEvent);

 	DWORD	dwExitCode;
	int count = 0;
 	while(::GetExitCodeThread(this->m_hThread, &dwExitCode) == TRUE &&
  	dwExitCode == STILL_ACTIVE && ( count < 10 ) )
   	{
		++count;
   		::WaitForSingleObject( this->m_hThread, 50 );
   	} 
 	
	//::TerminateThread(m_hThread , 0 );

//	::WaitForSingleObject(this->m_hThread, INFINITE);

	::CloseHandle(this->m_hThread);
	this->m_hThread = NULL;
}

// DS:: Sets the priority of the current thread.
BOOL GdsThread::SetThreadPriority(int nPriority)
{
	if(!this->m_hThread)
		return false;

	// Setting thread priority
	this->m_nPriority = nPriority;

	return ::SetThreadPriority(this->m_hThread, nPriority);
}

// DS:: Decrements a thread¡¯s suspend count.
DWORD GdsThread::ResumeThread()
{
	if(!this->m_hThread)
		return 0xFFFFFFFF;

	return ::ResumeThread(this->m_hThread);
}

// DS:: Increments a thread¡¯s suspend count.
DWORD GdsThread::SuspendThread()
{
	if(!this->m_hThread)
		return 0xFFFFFFFF;

	return ::SuspendThread(this->m_hThread);
}

// DS:: Window process thread.
bool GdsThread::WinProcessThread()
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
bool GdsThread::ProcessThread()
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
				SetRunProcess( TRUE );
				this->Run();
				SetRunProcess( FALSE );
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
void GdsThread::Run()
{
	// Will be overriden by subclass.
}

// Override to perform thread instance initialization.
BOOL GdsThread::InitInstance()
{
	// Create synchronization object.
	SAFE_DELETE(this->m_pSyncObject);
	this->m_pSyncObject = GdsSyncCriticalSection::New();
	this->m_pSyncObject->CreateSynchronize();

	return TRUE;
}

// Override to clean up when your thread terminates.
void GdsThread::ExitInstance()
{
}

// DS:: Enter lock.
void GdsThread::EnterLock()
{
	if(this->m_pSyncObject)
		this->m_pSyncObject->EnterLock();
}

// DS:: Leave lock.
void GdsThread::LeaveLock()
{
	if(this->m_pSyncObject)
		this->m_pSyncObject->LeaveLock();
}

// DS:: Wating blocking event.
void GdsThread::WaitingBlockingEvent(HANDLE *pEvent, DWORD dwTimeOut)
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
			//kOutputMsg01("GdsThread()...Abandoned...");
			return;
		case WAIT_TIMEOUT :
			//kOutputMsg01("GdsThread()...Time out...");
			break;
		default :
			//kOutputMsg01("GdsThread()...Unknown...");
			break;
		}
		::Sleep(1);
	}
}