#ifndef __SNOWBOARD_UTILITY_BGTHREAD_H__
#define __SNOWBOARD_UTILITY_BGTHREAD_H__

#include "../../Framework/Snowboard_stdafx.h"
#include "SnowThread.h"

class BGThread : public SnowThread
{

	enum{
		MAX_SIZE = 10,
	};
	typedef struct WORK_TOKEN
	{
		CSnow*				pIntance;
		void (CSnow::*pFunc)();
	};

	std::queue< WORK_TOKEN >	m_ThreadQueue;
//
// Initialize & Destroy Methods
public:
	static BGThread *New();

	BGThread();
	virtual ~BGThread(); 
	BGThread(const BGThread&) {} 
	void operator=(const BGThread&) {} 


//
// Member Functions
public:

	void	Push( CSnow* pInstance , void (CSnow::*pFunc)() );
	virtual void Run();

};


#endif	// __SNOWBOARD_UTILITY_SYNC_H__