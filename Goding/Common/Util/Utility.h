
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

	// ���� ��Ŀ
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

	template< typename Locker , typename Lock , typename Target >
	AutoLockAccesser<Locker , Lock , Target> AutoLock( Lock* lock , Target& target )
	{
		return AutoLockAccesser<Locker , lock , target>( lock , target );
	}
/*  ���� usage
	// Step 1. �Ӱ迵�� ��ü�� ���ÿ� �ø���.
	CRITICAL_SECTION cs;        
	// Step 2. �Ӱ迵�� ��ü�� �ʱ�ȭ �Ѵ�.
	InitializeCriticalSection( &cs );   
	// case 1. �Լ� ���� ����
	(*AutoLock<CSLocker>(&cs, f))( 1 );
	(*AutoLock<CSLocker>(&cs, f))( 2 );
	// case 2. ��ü ���� ����
	OBJ obj;
	AutoLock<CSLocker>( &cs, obj )->set( 1, 2, 3 );

	int a, b, c;
	boost::tie(a, b, c) = AutoLock<CSLocker>( &cs, obj )->get();

	std::cout << 
		AutoLock<CSLocker>( &cs, boost::tie( a, b, c ) )->get<2>() << "\n";

	// case 3. ���� ���� ����
	*AutoLock<CSLocker>( &cs, a ) = 3;
	*AutoLock<CSLocker>( &cs, b ) = 4;
	*AutoLock<CSLocker>( &cs, c ) = 5;

	// case 4. vector ���� ����
	std::vector<int> vec;
	AutoLock<CSLocker>( &cs, vec )->push_back( 3 );
	AutoLock<CSLocker>( &cs, vec )->back();


	// Step 5. �Ӱ迵�� ��ü�� �����Ѵ�.
	DeleteCriticalSection( &cs );
*/

	//��� ���̺�
	template< class KEY , class T >
	class GdsLookupTable
	{
		typedef pair< KEY , T > VALUE;
		typedef vector< VALUE > CONTAINER;
		CONTAINER		m_vecData;

		bool			m_isSorted;

	public:

		class DataCompare
		{
		public:
			bool operator()( const VALUE& lhs , const VALUE& rhs ) const
			{
				return lhs.first < rhs.first;
			}
			bool operator()( const VALUE& lhs , const KEY& rhs ) const 
			{
				return lhs.first < rhs;
			}
			bool operator()( const KEY& lhs , const VALUE& rhs ) const
			{
				return lhs < rhs.first;
			}
		};


		GdsLookupTable(){ clear(); }
		~GdsLookupTable(){ clear(); }

		void clear()
		{
			m_isSorted = false;
			m_vecData.clear();
		}

		bool isUnique()
		{
			return ( m_vecData.end() == adjacent_find(m_vecData.begin(), m_vecData.end()));
		}

		void add( KEY key , T val )
		{
			VALUE data;
			data.first = key;
			data.second = val;
			m_vecData.push_back( data );				
		}	

		bool empty()
		{
			return m_vecData.empty();
		}

		T* find( KEY key )
		{
			ASSERT( m_isSorted );	
			CONTAINER::iterator i = lower_bound( m_vecData.begin() , m_vecData.end() , key , DataCompare() );
			if ( i != m_vecData.end() && i->first == key )
			{
				//return &m_vecData[ distance( m_vecData.begin() , i ) ].second;
				return &(i->second);
			}
			else
			{
				return NULL;
			}		
		}

		void sort()
		{	
			CONTAINER( m_vecData ).swap( m_vecData );
			std::sort( m_vecData.begin() , m_vecData.end() , DataCompare() );
			ASSERT( isUnique() && "Ű�� �ߺ�" );

			m_isSorted = true;
		}
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
		void operator() (TCHAR& c) const { c = static_cast<TCHAR>(tolower(c)); }
	};
}// end of namespace functor

using namespace util;