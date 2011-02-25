
#pragma once

#include "../Framework/Snowboard_stdafx.h"

namespace util
{
	namespace string
	{
		enum eCodePage{
			Japan = 932,		// 일본어
			China = 936,		// 중국어(간체)
			Korea = 949,		// 한국어
			Taiwan = 950,		// 중국어(번체)
			English = 1252,		// 라틴1, 일반적인 영어 문자
			utf_8	= 65001,

			DefaultCodePage = Korea,	// 기본
		};
		
		//---------------------------------------------------------------------------------------------
		// MBCS -> UNICODE
		bool mb2wc( wchar_t * buffer, const char * in, size_t bufferLength, eCodePage codepage = DefaultCodePage );
		std::wstring mb2wc( std::string const & ms, eCodePage codepage = DefaultCodePage );
		std::wstring mb2wc( char const * ms, eCodePage codepage = DefaultCodePage );

		//---------------------------------------------------------------------------------------------
		// UNICODE -> MBCS
		bool wc2mb( char * buffer, const wchar_t * in, size_t bufferLength, eCodePage codepage = DefaultCodePage );
		std::string wc2mb( std::wstring const & ws, eCodePage codepage = DefaultCodePage );
		std::string wc2mb( wchar_t const * ws, eCodePage codepage = DefaultCodePage );

		std::wstring GetLastErrorString(wchar_t const * file=L"where", uint32_t line=0xffff);

		std::wstring make_string(const wchar_t * fmt, ...);
		std::string make_string(const char * fmt, ...);
		bool str2bool (wchar_t const * s);
	}// end of namespace string

	///! from boost::noncopyable
	class noncopyable
	{
	protected:
		noncopyable() {}
		~noncopyable() {}
	private:  // emphasize the following members are private
		noncopyable( const noncopyable& );
		const noncopyable& operator=( const noncopyable& );
	};

	// 오토 락커
	class CSLocker
	{
	private:
		CSLocker( const CSLocker& );
		CSLocker& operator=( const CSLocker& );
	public:
		CSLocker( CRITICAL_SECTION* pcs ) : pcs_( pcs )
		{
			EnterCriticalSection( pcs_ );
		}
		~CSLocker()
		{
			LeaveCriticalSection( pcs_ );
		}
	private:
		CRITICAL_SECTION* pcs_;
	};

	template< typename Locker , typename Lock , typename Target >
	class AutoLockAccesser
	{
	public:
		AutoLockAccesser( Lock* plock , Target& target )
			: locker_( plock ) , target_( target )
		{
		}
		Target& operator*()
		{
			return target_;
		}
		Target* operator->()
		{
			return &target_;
		}
	private:
		Target& target_;
		Locker locker_;
	};
}// end of namespace util

namespace functor
{
	struct deleter{
		template<typename T> void operator() (T*& t){
			if(t){delete t; t=NULL;}
		}
		template<typename KEY, typename VALUE> 
		void operator() (std::pair<const KEY, VALUE>& t){
			if(t.second){delete t.second; t.second=NULL;}
		}
	};
	struct array_deleter{
		template<typename T> void operator() (T*& t){
			if(t){delete [] t; t=NULL;}
		}
		template<typename KEY, typename VALUE> 
		void operator() (std::pair<const KEY, VALUE>& t){
			if(t.second){delete [] t.second; t.second=NULL;}
		}
	};

	struct update{
		template<typename T> void operator()(T*& t){
			if(t){ t->Update(); }
		}
		template<typename KEY, typename VALUE> 
		void operator() (std::pair<const KEY, VALUE>& t){
			if(t.second){ t.second->Update(); }
		}
	};

	struct release{
		template<typename T> void operator() (T*& t){
			if(t){ t->Release(); }
		}
		template<typename KEY, typename VALUE> 
		void operator() (std::pair<const KEY, VALUE>& t){
			if(t.second){ t.second->Release(); }
		}
	};

	struct removeallchild{
		template<typename T> void operator() (T*& t){
			if(t){ t->RemoveAllChild(); }
		}
		template<typename KEY, typename VALUE> 
		void operator() (std::pair<const KEY, VALUE>& t){
			if(t.second){ t.second->RemoveAllChild(); }
		}
	};

	struct ToLower{ 
		void operator() (TCHAR& c) const { c = static_cast<TCHAR>(tolower(c)); }
	};
}// end of namespace functor

