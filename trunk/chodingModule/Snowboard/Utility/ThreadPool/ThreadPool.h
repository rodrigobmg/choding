#ifndef __SNOWBOARD_UTILITY_THREADPOOL__
#define __SNOWBOARD_UTILITY_THREADPOOL__

#include "../../Framework/Snowboard_stdafx.h"
#include "../singleton.h"
#include "BGThread.h"


class ThreadPool : public singleton< ThreadPool >
{
private:

	enum{
		DEFAULT_CAPACITY = 10,
	};

	typedef std::list< BGThread* >		THREAD_CONTAINER;
	THREAD_CONTAINER					m_ThreadPool;

	BGThread*							m_BgThread;

public:
	ThreadPool();
	virtual ~ThreadPool();

	BGThread*		GetBGThread();
	void			SetBGThreadMaxCapacity( BYTE value );
	BYTE			GetBGThreadMaxCapacity();

	void			Create( int count  );
	void			Clear();

	BGThread*		GetIdleThread();
};

#endif