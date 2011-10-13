#include "stdafx.h"

#ifdef _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK , __FILE__ , __LINE__ )
static class MemoryLeakCheck
{
public:
	MemoryLeakCheck()
	{
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	}
	~MemoryLeakCheck()
	{
		_ASSERTE( _CrtCheckMemory() );
	}
} MemoryLeak;
#endif

GdsMemObject::GdsMemObject()
{
	//LOG_INFO_F( "MemAlloc %s " ,  __FUNCTION__ );
}

GdsMemObject::~GdsMemObject()
{


	//LOG_BLUE_F( "MemFree" );
}

/*
#ifndef _CRTDBG_MAP_ALLOC


void* GdsMemObject::operator new( size_t size )
{
	size_t size1 = sizeof( GdsMemObject );
	size_t size2 = sizeof( GdsGameObject );
	char* p = new char[size];
	return (void*)p;
}

void* GdsMemObject::operator new[]( size_t size )
{
	size_t size1 = sizeof( GdsMemObject );
	size_t size2 = sizeof( GdsGameObject );
	char* p = new char[size];
	return (void*)p;
}

#else

void* GdsMemObject::operator new( size_t size , int a , const char* sz , int i)
{
	size_t size1 = sizeof( GdsMemObject );
	size_t size2 = sizeof( GdsGameObject );
	char* p = new char[size];
	return (void*)p;
}

void* GdsMemObject::operator new[]( size_t size , int a , const char* sz , int i)
{
	size_t size1 = sizeof( GdsMemObject );
	size_t size2 = sizeof( GdsGameObject );
	char* p = new char[size];
	return (void*)p;
}


#endif

void GdsMemObject::operator delete( void* p , size_t size )
{
	delete p;
}

void GdsMemObject::operator delete[]( void* p , size_t size )
{
	delete[] p;
}*/

/*
#ifndef __POOL_HPP
#  define __POOL_HPP
#include <windows.h>

#include "pool.h"
#include <cstddef>

# include <new>
# include <stdexcept>

#undef max
#include <limits>


namespace pool {
	template<typename T>
	class allocator {
	public: 
		//    typedefs
		typedef T value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type; 

	public: 
		//    convert an allocator<T> to allocator<U>
		template<typename U>
		struct rebind { typedef allocator<U> other; }; 

	public: 
		inline allocator() {}
		inline ~allocator() {}

		inline allocator( allocator const&) {}
		template<typename U>
		inline allocator( allocator<U> const&) {} 

		//    address
		inline pointer address( reference r) { return &r; }
		inline const_pointer address( const_reference r) { return &r; } 

		//    memory allocation
		inline pointer allocate( size_type cnt, 
			typename std::allocator<void>::const_pointer p = 0) { 
				struct POOL *pl;

				((_S_indexOfTLS == TLS_OUT_OF_INDEXES) ? (_S_indexOfTLS = TlsAlloc()): 0);
				if ((pl = (struct POOL *)TlsGetValue( _S_indexOfTLS)))
					;
				else {
					if (!(pl = pool_create( 0, NULL)))
						throw std::bad_alloc();
					else {
						if (TlsSetValue( _S_indexOfTLS, pl) < 0)
							throw std::runtime_error("TlsSetValue");
					}
				} return reinterpret_cast<pointer>( pool_resize( pl, 
					const_cast<void *>(p), cnt * sizeof(T), 0));
		}

		inline void deallocate( pointer p, size_type l) {
			struct POOL *pl;

			if ((_S_indexOfTLS != TLS_OUT_OF_INDEXES) &&
				((pl = (struct POOL *)TlsGetValue( _S_indexOfTLS))))
			{
				if (p) pool_put( pl, reinterpret_cast<void *>(p));
				else if (l < 0) // 해당 쓰레드에 할당된 메모리풀 제거
				{
					pool_destroy( pl);
					TlsSetValue( _S_indexOfTLS, NULL);
				}
			}
		}

		//    size
		inline size_type max_size() const { 
			return std::numeric_limits<size_type>::max() / sizeof(T); } 

		//    construction/destruction
		inline void construct( pointer p, const T& t) { new(p) T(t); }
		inline void destroy( pointer p) { p->~T(); }

	private:
		static DWORD _S_indexOfTLS;
	};  //    end of class allocator

	template<typename T> DWORD allocator<T>::_S_indexOfTLS = TLS_OUT_OF_INDEXES;

	template<typename T>
	inline bool operator == ( allocator<T> const &, allocator<T> const &) {
		return true; }
	template<typename T>
	inline bool operator != ( allocator<T> const &, allocator<T> const &) {
		return false; }

	static allocator<char> standard;

	// 쓰레드에 할당된 리소스를 반환 받는다.
	inline void destroy() { standard.deallocate( NULL, -1); }

	//
	//
	template<class _Allocator>
	class safer {
	public:
		void *operator new   (size_t l) { return (void *)_Allocator.allocate(l); }
		void  operator delete( void *p) { _Allocator.deallocate( (char *)p, 0); }

		void *operator new[]   (size_t l) { return operator new(l); }
		void  operator delete[]( void *p) { operator delete(p); }
	};

	// scoped<char> buffer(100);
	//
	template <typename Tp>
	class scoped {
	public:
		scoped( std::size_t l = 1)
		{
			if (!(_M_buffer = (Tp *)standard.allocate( _M_sizeof = l)))
				throw std::bad_alloc();
		}

		~scoped() { standard.deallocate( _M_buffer, 0); }

		Tp *resize( std::size_t l)
		{
			Tp *buffer;

			if (!(buffer = (Tp *)standard.allocate( l, (char *)_M_buffer)))
				throw std::bad_alloc();
			else {
				_M_sizeof = l;
			} return (_M_buffer = buffer);
		}

		Tp *operator *() { return _M_buffer; }
		Tp *operator []( int offset)
		{
			if ((offset < 0) ||
				(offset >= _M_sizeof))
				throw std::out_of_range("operator []: out of range");

			return (_M_buffer + offset);
		}

		const int size() { return _M_sizeof; }
	protected:
		Tp *_M_buffer;
		std::size_t _M_sizeof;
	};
};
#endif*/
