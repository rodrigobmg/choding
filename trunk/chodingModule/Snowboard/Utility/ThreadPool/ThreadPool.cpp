#include "ThreadPool.h"


ThreadPool::ThreadPool()
: m_BgThread(NULL)
{
	Clear();
}

ThreadPool::~ThreadPool()
{
	Clear();
}

void ThreadPool::Clear()
{
	THREAD_CONTAINER::iterator it = m_ThreadPool.begin();
	for ( ; it != m_ThreadPool.end() ; ++it )
	{
		SAFE_DELETE( (*it) );
	}
	m_ThreadPool.clear();

	SAFE_DELETE( m_BgThread );
}

void ThreadPool::SetBGThreadMaxCapacity( BYTE byvalue )
{
	if ( m_BgThread == NULL )
		assert( 0 && "백그라운더 할당후 해주세요" );

	m_BgThread->SetMaxcapacity( byvalue );
}

BYTE ThreadPool::GetBGThreadMaxCapacity()
{
	if ( m_BgThread == NULL )
		assert( 0 && "백그라운더 할당후 해주세요" );

	return m_BgThread->GetMaxcapacity();
}

void ThreadPool::Create( int count )
{
	for ( size_t t = 0 ; t < (size_t)count ; ++t )
	{
		BGThread* p = new BGThread;
		p->CreateAndRunThread();
		m_ThreadPool.push_back( p );
	}	
}

BGThread* ThreadPool::GetIdleThread()
{
	THREAD_CONTAINER::iterator it = m_ThreadPool.begin();
	for ( ; it != m_ThreadPool.end() ; ++it )
	{
 		if ( (*it)->GetThreadDone() == FALSE )
 			return (*it);
	}

	return NULL;
}

BGThread* ThreadPool::GetBGThread()
{
	if ( m_BgThread )
		return m_BgThread;

	m_BgThread = new BGThread;
	m_BgThread->CreateAndRunThread();
	m_BgThread->SetMaxcapacity( 10 );
	return m_BgThread;
}