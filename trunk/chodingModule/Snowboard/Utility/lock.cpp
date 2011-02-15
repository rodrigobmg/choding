#include "lock.h"

namespace util
{
	Lock::Lock()
		D_LINE : count_(0)
		D_LINE , thread_id_(-1)
		D_LINE , line_(-1)
		D_LINE , file_name_(NULL)
		D_LINE , func_name_(NULL)
	{
		VERIFY( InitializeCriticalSectionAndSpinCount(&cs_, 4000) );
	}
	Lock::~Lock()
	{
		::DeleteCriticalSection(&cs_);
	};

	void Lock::lock(
		D_LINE int32_t line
		D_LINE , char const * file_name
		D_LINE , char const * func_name
		)
	{
		::EnterCriticalSection(&cs_);

		//ASSERT(0==count_);
		D_LINE ++count_;
		D_LINE thread_id_ = GetCurrentThreadId();
		D_LINE line_ = line;
		D_LINE file_name_ = file_name;
		D_LINE func_name_ = func_name;
	}

	void Lock::unlock()
	{
		//ASSERT(1 == count_);
		D_LINE --count_;

		::LeaveCriticalSection(&cs_);
	}

	ScopeLock::ScopeLock( Lock & lock_

		D_LINE , int32_t line
		D_LINE , char const * file_name
		D_LINE , char const * func_name

		) : lock_(lock_)
	{
		lock_.lock(
			D_LINE line
			D_LINE , file_name
			D_LINE , func_name
			);
	}
	ScopeLock::~ScopeLock(){lock_.unlock();}
}