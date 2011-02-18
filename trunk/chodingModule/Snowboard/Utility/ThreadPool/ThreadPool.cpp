#include "ThreadPool.h"


ThreadPool::ThreadPool()
{
	m_BgThread = BGThread::New();
}

ThreadPool::~ThreadPool()
{

}
