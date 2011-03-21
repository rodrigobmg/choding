#ifndef __SNOWBOARD_UTILITY_GDSTHREADPOOL__
#define __SNOWBOARD_UTILITY_GDSTHREADPOOL__

#include "../../Framework/Snowboard_stdafx.h"
#include "../Singleton/singleton.h"
#include "GdsBGThread.h"


class GdsThreadPool
{
private:

	enum{
		DEFAULT_CAPACITY = 10,
	};

	typedef std::list< GdsBGThread* >		THREAD_CONTAINER;
	THREAD_CONTAINER						m_GdsThreadPool;

	GdsBGThread*							m_GdsBGThread;

public:
	GdsThreadPool();
	virtual ~GdsThreadPool();

	GdsBGThread*		GetBGThread();
	void				SetBGThreadMaxCapacity( BYTE value );
	BYTE				GetBGThreadMaxCapacity();

	void				Create( int count  );
	void				Clear();

	GdsBGThread*		GetIdleThread();
};

typedef boost::shared_ptr< GdsThreadPool >	GdsThreadPoolPtr;

#endif