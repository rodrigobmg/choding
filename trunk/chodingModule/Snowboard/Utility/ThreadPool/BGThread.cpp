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

		// 작업이 끝난후 큐에 작업이 있다면 이벤트한번더~
		if ( !m_ThreadQueue.empty() )
			::SetEvent( m_hEvent );
	}
}

void BGThread::Push( /*CSnow* pInstance , void (CSnow::*pf)()*/ )
{
	if ( m_ThreadQueue.size() < MAX_SIZE )
	{
		WORK_TOKEN wt;
		wt.pIntance = 0;
		wt.pFunc	= 0;
		m_ThreadQueue.push( wt );
		::SetEvent( m_hEvent );
	}
	else
		assert( 0 && "너무 많이 등록함" );
}