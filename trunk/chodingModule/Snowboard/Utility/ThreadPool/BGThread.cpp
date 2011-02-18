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
	/*
	if ( m_ThreadFunctorQueue.empty() )
			return;
	
		TFunctor* pFunctor = m_ThreadFunctorQueue.front();
		if ( pFunctor )
		{
			(*pFunctor)( L"test" );
			// �۾��� ������ ť�� �۾��� �ִٸ� �̺�Ʈ�ѹ���~
			if ( !m_ThreadFunctorQueue.empty() )
				::SetEvent( m_hEvent );
		}*/
	
}

void BGThread::Push( CSnow* pInstance , void (CSnow::*pf)() )
{
	if ( m_ThreadQueue.size() < MAX_SIZE )
	{
		WORK_TOKEN wt;
		wt.pIntance = pInstance;
		wt.pFunc	= pf;
		m_ThreadQueue.push( wt );
		::SetEvent( m_hEvent );
	}
	else
		assert( 0 && "�ʹ� ���� �����" );
}
/*
template<typename T >
void BGThread::Push( TSpecificFunctor<T>* tFunctor )
{
	if ( m_ThreadFunctorQueue.size() < MAX_SIZE )
	{
		m_ThreadFunctorQueue.push( tFunctor );
		::SetEvent( m_hEvent );
	}
	else
		assert( 0 && "�ʹ� ���� �����" );
}*/
