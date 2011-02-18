#ifndef __SNOWBOARD_UTILITY_THREADPOOL__
#define __SNOWBOARD_UTILITY_THREADPOOL__

#include "../../Framework/Snowboard_stdafx.h"
#include "../singleton.h"
#include "BGThread.h"
#include "SnowThread.h"

class ThreadPool : public singleton< ThreadPool >
{
private:

	enum{
		DEFAULT_CAPACITY = 10,
	};
	
	BGThread*					m_BgThread;

	std::vector< SnowThread* >	m_ThreadPool;

public:
	ThreadPool();
	virtual ~ThreadPool();
	ThreadPool(const ThreadPool&) {} 
	void operator=(const ThreadPool&) {} 

	BGThread*	GetBGThread()	{ return m_BgThread; }

// 	void		Push();
// 	void		Pop();
// 
// 	void		SetCapacity();
// 	size_t		GetCapacity();
};


#endif