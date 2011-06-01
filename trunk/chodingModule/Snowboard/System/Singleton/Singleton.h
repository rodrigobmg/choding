#ifndef _GDS_SYSTEM_SINGLETON__POLICY_BAESD_H_
#define _GDS_SYSTEM_SINGLETON__POLICY_BAESD_H_

#include "../../Framework/Snowboard_stdafx.h"

#ifndef _WIN32_WINNT  // Windows XP 이상에서만 기능을 사용할 수 있습니다.                   
#define _WIN32_WINNT 0x0501 // 다른 버전의 Windows에 맞도록 적합한 값으로 변경해 주십시오.
#endif   

// 기본 정책기반 싱글톤
// create policy
template <typename T>
class create_using_new 
{
public:
	static T* create()
	{
		return new T();
	}

	static void destroy(T *p)
	{
		delete p;
	}
};

template <typename T>
class create_using_static
{
public:
	static T* create()
	{
		static static_memory t;
		return new (&T)T;
	}
	static void destroy(T *p)
	{
		p->~T();
	}
private:
	union static_memory
	{
		char temp[sizeof(T)];
	};

};

// life time policy
//
typedef void (*atexit_pfn_t)();

template <typename T>
class default_life_time
{
public:
	static void on_dead_reference()
	{
		throw std::logic_error("dead reference");
	}
	static void schedule_destruction(T *, atexit_pfn_t pfn)
	{
		std::atexit(pfn);
	}
};

template <typename T>
class phoenix_life_time
{
public:
	static void on_dead_reference()
	{
		//destroyed_once_ = true;
	}

	static void schedule_destruction(T *, atexit_pfn_t pfn)
	{
		//if (false == destroyed_once_)
		std::atexit(pfn);
	}

private:
	//static bool destroyed_once_;
};

//template <typename T> bool phoenix_life_time<T>::destroyed_once_ = false;


template <typename T, typename MT>
class single_thread
{
public:
	// dummy locker for single thread
	struct locker
	{
		locker() {}
		explicit locker(const single_thread&){}
		explicit locker(const single_thread*){}
	};

	typedef T* volatile_ptr_t;
};

template <typename T, typename MT>
class multi_thread
{
public:
	typedef T* volatile_ptr_t;

public:
	struct locker
	{
		~locker()
		{
			mt_.un_lock();
		}
		locker()
		{
			mt_.lock();
		}
	};

private:
	static MT mt_;
};

template <typename T, typename MT>
MT multi_thread<T, MT>::mt_;

#if defined(WIN32) || defined(WIN64)

class default_mutex
{
private:
	default_mutex(const default_mutex&);
	default_mutex& operator=(const default_mutex&);

public:
	~default_mutex()
	{
		DeleteCriticalSection(&critical_section_);
	}
	default_mutex()
	{
		//InitializeCriticalSectionAndSpinCount(&critical_section_, 4000 );
		InitializeCriticalSection( &critical_section_ );
	}

	void lock()
	{
		EnterCriticalSection(&critical_section_);
	}
	void un_lock()
	{
		LeaveCriticalSection(&critical_section_);
	}
private:
	CRITICAL_SECTION critical_section_;
};
#endif

template <typename T, 
template <class> class creation_policy = create_using_new,
template <class> class life_time_policy = default_life_time,
template <class, class> class thread_policy = single_thread,
typename MUTEX_T = default_mutex
>
class singleton_holder
{
private:
	~singleton_holder()
	{
	}
	singleton_holder()
	{
	}
public:	

	static void destroy_singleton()
	{
		creation_policy<T>::destroy(p_instance_);
		p_instance_ = 0;
		destoryed_ = true;
	}

	static T& instance()
	{
		if (0 == p_instance_)
		{
			typename thread_policy<T, MUTEX_T>::locker guard;
			(void)guard;

			if (0 == p_instance_)
			{
				if (true == destoryed_)
				{
					default_life_time<T>::on_dead_reference();
					destoryed_ = false;
				}

				p_instance_ = creation_policy<T>::create();
				life_time_policy<T>::schedule_destruction(p_instance_, destroy_singleton);
			}
		}
		return *p_instance_;
	}

private:
	typedef typename thread_policy<T, MUTEX_T>::volatile_ptr_t instance_ptr_t;
	static instance_ptr_t p_instance_;
	static bool destoryed_;
};

template <typename T, 
template <class> class C,
template <class> class L,
template <class, class> class TM,
typename MT
>
typename singleton_holder<T, C, L, TM, MT>::instance_ptr_t 
singleton_holder<T, C, L, TM, MT>::p_instance_ = 0;

template <typename T, 
template <class> class C,
template <class> class L,
template <class, class> class TM,
typename MT
>
bool singleton_holder<T, C, L, TM, MT>::destoryed_ = false;


#endif 

