#include "GdsThreadPool.h"

GdsThreadPool::GdsThreadPool()
{
	clear();
	create();
}

GdsThreadPool::~GdsThreadPool()
{
	clear();
}

void GdsThreadPool::clear()
{
	THREAD_CONTAINER::iterator it = m_GdsThreadPool.begin();
 	for ( ; it != m_GdsThreadPool.end() ; ++it )
 	{
		(*it)->DestroyThread();
 		SAFE_DELETE( (*it) );
 	}
	m_GdsThreadPool.clear();
}

void GdsThreadPool::create()
{
	SYSTEM_INFO info;
	GetSystemInfo( &info );

	for ( size_t t = 0 ; t < info.dwNumberOfProcessors*2 ; ++t )
	{
		GdsBGThread* p = new GdsBGThread;
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

GdsBGThread* GdsThreadPool::GetThread( int index )
{
	int size = static_cast<int>( m_GdsThreadPool.size() );
	if ( index > size )
		return NULL;

	return m_GdsThreadPool.at( index );
}