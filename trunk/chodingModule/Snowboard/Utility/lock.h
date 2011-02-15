#pragma once

#include <Windows.h>
#include "Utility.h"

namespace util
{
	class Lock
	{
		CRITICAL_SECTION cs_;

		D_LINE int32_t count_;
		D_LINE int32_t thread_id_;

		D_LINE int32_t line_;
		D_LINE char const * file_name_;
		D_LINE char const * func_name_;

	public:
		Lock();
		~Lock();

		void lock(
			D_LINE int32_t line
			D_LINE , char const * file_name
			D_LINE , char const * func_name
			);

		void unlock();
	};


	class ScopeLock : noncopyable
	{
		Lock & lock_;

	public:
		ScopeLock( Lock & lock_

			D_LINE , int32_t line
			D_LINE , char const * file_name
			D_LINE , char const * func_name

			);

		~ScopeLock();
	};
}

#ifdef _DEBUG
#	define SCOPE_LOCK(theLock)	ScopeLock MAKE_NAME(scope_lock)(theLock, __LINE__, __FILE__, __FUNCSIG__); MAKE_NAME(scope_lock)
#	define LOCK(theLock)		theLock.lock(__LINE__, __FILE__, __FUNCSIG__)
#	define UNLOCK(theLock)		theLock.unlock()
#else
#	define SCOPE_LOCK(theLock)	ScopeLock MAKE_NAME(scope_lock)(theLock); MAKE_NAME(scope_lock)
#	define LOCK(theLock)		theLock.lock()
#	define UNLOCK(theLock)		theLock.unlock()
#endif //_DEBUG
