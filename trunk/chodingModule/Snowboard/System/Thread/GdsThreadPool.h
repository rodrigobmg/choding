#ifndef __SNOWBOARD_UTILITY_GDSTHREADPOOL__
#define __SNOWBOARD_UTILITY_GDSTHREADPOOL__

#include "../../Framework/Snowboard_stdafx.h"
#include "../Singleton/singleton.h"
#include "GdsBGThread.h"


class GdsThreadPool
{
public:
	GdsThreadPool();
	virtual ~GdsThreadPool();

	int					GetTotalCountofThread()const{ return (int)m_GdsThreadPool.size(); }	
	GdsBGThread*		GetThread( int index );
	GdsBGThread*		GetIdleThread();

private:

	typedef std::vector< GdsBGThread* >		THREAD_CONTAINER;
	THREAD_CONTAINER						m_GdsThreadPool;
	
	void				create();
	void				clear();

};

#define THREADPOOL	singleton_holder< GdsThreadPool >::instance()

#endif