#include "BGThread.h"


/////////////////////////////////////////////////////////////////////////////
// Initialize & Destroy Methods

// DS:: Create an instance of this class.
BGThread *BGThread::New()
{
	// Create it here.
	return new BGThread;
}

// DS:: Constructor.
BGThread::BGThread()
{
	SetName( OBJECT_BGTHREAD );
}

// DS:: Destructor.
BGThread::~BGThread()
{
}

void BGThread::Run()
{
	if ( m_ThreadQueue.empty() )
		return;

	WORK_TOKEN wt = m_ThreadQueue.front();
	if( wt.pIntance != NULL && wt.pFunc != NULL )
	{
		(wt.pIntance->*wt.pFunc)();
		m_ThreadQueue.pop();
	}
}

void BGThread::Push( CSnow* pInstance , void (CSnow::*pf)() )
{
	this->EnterLock();
	if ( m_ThreadQueue.size() < MAX_SIZE )
	{
		WORK_TOKEN wt;
		wt.pIntance = pInstance;
		wt.pFunc	= pf;
		m_ThreadQueue.push( wt );
	}
	else
		assert( 0 && "너무 많이 등록함" );
	this->LeaveLock();
}