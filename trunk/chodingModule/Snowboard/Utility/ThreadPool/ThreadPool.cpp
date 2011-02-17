#include "ThreadPool.h"


ThreadPool::ThreadPool()
{

}

ThreadPool::~ThreadPool()
{

}

ThreadPool* ThreadPool::New()
{
	return new ThreadPool;
}