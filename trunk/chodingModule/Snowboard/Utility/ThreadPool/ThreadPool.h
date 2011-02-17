#ifndef __SNOWBOARD_UTILITY_THREADPOOL__
#define __SNOWBOARD_UTILITY_THREADPOOL__

#include "../../Framework/Snowboard_stdafx.h"
#include "../../Core/Base/Snow.h"


class ThreadPool : public CSnow
{
private:

// 	typedef
// 
// 	std::list<

public:
	ThreadPool();
	virtual ~ThreadPool();

	static ThreadPool*	New();

};


#endif