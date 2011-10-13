
#pragma once

namespace util
{
	namespace string
	{
		enum eCodePage{
			Japan = 932,		// �Ϻ���
			China = 936,		// �߱���(��ü)
			Korea = 949,		// �ѱ���
			Taiwan = 950,		// �߱���(��ü)
			English = 1252,		// ��ƾ1, �Ϲ����� ���� ����
			utf_8	= 65001,

			DefaultCodePage = Korea,	// �⺻
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

		std::wstring GetLastErrorString(wchar_t const * file=L"where", int line=0xffff);

		std::wstring make_string(const wchar_t * fmt, ...);
		std::string make_string(const char * fmt, ...);
		bool str2bool (wchar_t const * s);
	}// end of namespace string

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
			if(t){ t->Release(); t = NULL; }
		}
		template<typename KEY, typename VALUE> 
		void operator() (std::pair<const KEY, VALUE>& t){
			if(t.second){ t.second->Release(); t.second = NULL; }
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
		void operator() (char& c) const { c = static_cast<char>(tolower(c)); }
	};

	struct ToLowerW{ 
		void operator() (wchar_t& c) const { c = static_cast<wchar_t>(tolower(c)); }
	};

}// end of namespace functor

using namespace util;