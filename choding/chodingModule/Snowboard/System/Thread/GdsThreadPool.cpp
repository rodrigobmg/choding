#include "GdsThreadPool.h"

GdsThreadPool::GdsThreadPool()
: m_GdsBGThread(NULL)
{
	Clear();
}

GdsThreadPool::~GdsThreadPool()
{
	Clear();
}

void GdsThreadPool::Clear()
{
	THREAD_CONTAINER::iterator it = m_GdsThreadPool.begin();
 	for ( ; it != m_GdsThreadPool.end() ; ++it )
 	{
		(*it)->SuspendThread();
 		SAFE_DELETE( (*it) );
 	}
	m_GdsThreadPool.clear();

	if ( m_GdsBGThread )
		m_GdsBGThread->SuspendThread();
	SAFE_DELETE( m_GdsBGThread );
}

void GdsThreadPool::SetBGThreadMaxCapacity( BYTE byvalue )
{
	if ( m_GdsBGThread == NULL )
		ASSERT( 0 && "백그라운더 할당후 해주세요" );

	m_GdsBGThread->SetMaxcapacity( byvalue );
}

BYTE GdsThreadPool::GetBGThreadMaxCapacity()
{
	if ( m_GdsBGThread == NULL )
		ASSERT( 0 && "백그라운더 할당후 해주세요" );

	return m_GdsBGThread->GetMaxcapacity();
}

void GdsThreadPool::Create( int count )
{
	for ( size_t t = 0 ; t < (size_t)count ; ++t )
	{
		GdsBGThread* p = new GdsBGThread;
		//GdsBGThreadPtr p = GdsBGThreadPtr( new GdsBGThread );
		p->CreateAndRunThread();
		m_GdsThreadPool.push_back( p );
	}	
}

GdsBGThread* GdsThreadPool::GetIdleThread()
{
	THREAD_CONTAINER::iterator it = m_GdsThreadPool.begin();
	for ( ; it != m_GdsThreadPool.end() ; ++it )
	{
 		if ( (*it)->GetThreadDone() == FALSE )
 			return (*it);
	}

	return NULL;
}

GdsBGThread* GdsThreadPool::GetBGThread()
{
	if ( m_GdsBGThread )
		return m_GdsBGThread;

	m_GdsBGThread = new GdsBGThread;
	m_GdsBGThread->CreateAndRunThread();
	m_GdsBGThread->SetMaxcapacity( 10 );
	return m_GdsBGThread;
}
