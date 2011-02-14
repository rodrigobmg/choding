#pragma once
//#include "lock.h"
#include "../Framework/Snowboard_stdafx.h"
#include <list>

namespace util
{
	namespace PRIVATE
	{
	//	Lock & lock_for_process_singleton();


		class TStorageCleaner
		{
			typedef void (*functr_t)(void);
			typedef std::list<functr_t>	container_t;

			__declspec (thread) static container_t * pDestroyers_;

		public:
			static void onBeginThreadProc()
			{
				ASSERT(!pDestroyers_);
				pDestroyers_ = new container_t;
			}
			static void onExitThreadProc()
			{
				ASSERT(pDestroyers_);
				typedef container_t::iterator iterator;
				for(iterator itr=pDestroyers_->begin(); itr!=pDestroyers_->end(); ++itr)
					(*itr)();
				pDestroyers_->clear();
				SAFE_DELETE(pDestroyers_);
			}
			static void addDestoryer(functr_t f)
			{
				ASSERT(pDestroyers_);
				pDestroyers_->push_back(f);
			}
		};
	}//namespace PRIVATE

	template<class T, template <class> class Storage >	class Singleton;
	template<class T> class PStorage
	{
		static T * pInstance_;

		static void create(){
		//	SCOPE_LOCK(PRIVATE::lock_for_process_singleton());
			if(!pInstance_)
			{
				T * tmp(new T);
				pInstance_ = tmp;
				atexit(destroy);
			}
		}
		static void destroy()
		{
	//		SCOPE_LOCK(PRIVATE::lock_for_process_singleton());
			SAFE_DELETE(pInstance_);
		}

		template<class T, template <class> class Storage >
		friend class Singleton;
	};
	template<class T> T* PStorage<T>::pInstance_ = NULL;

	template<class T> class TStorage
	{
		__declspec(thread) static T *pInstance_;

		static void create()
		{
			ASSERT(!pInstance_);
			pInstance_ = new T;
			PRIVATE::TStorageCleaner::addDestoryer(destroy);
		}

		static void destroy()
		{
			SAFE_DELETE(pInstance_);
		}

		template<class T, template <class> class Storage >
		friend class Singleton;
	};
	template <class T> T* TStorage<T>::pInstance_ = NULL;

	template<class T, template <class> class Storage = PStorage >
	class Singleton
	{
	protected:
		typedef Storage<T>	storage_t;

	public:
		static T& getInstance()
		{
			if(!storage_t::pInstance_)
			{
				storage_t::create();
			}
			return *storage_t::pInstance_;
		}
	};

}//namespace util

#define USING_SINGLETON(_class) \
	private:					\
	friend class storage_t;		\
	_class();					\
	~_class()

#define ON_BEGIN_THREAD	::util::PRIVATE::TStorageCleaner::onBeginThreadProc
#define ON_EXIT_THREAD	::util::PRIVATE::TStorageCleaner::onExitThreadProc
