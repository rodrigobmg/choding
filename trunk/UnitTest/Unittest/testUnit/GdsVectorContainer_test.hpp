#ifndef _UNITTEST_GDSVECTORCONTAINER_H_
#define _UNITTEST_GDSVECTORCONTAINER_H_

class GdsVectorContainerTest : public ::testing::Test
{
public:

	template< class KEY , class T >
	class lookupTable
	{
		typedef pair< KEY , T > VALUE;
		typedef vector< VALUE > CONTAINER;
		CONTAINER	m_vecData;

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


		lookupTable(){ clear(); }
		~lookupTable(){ clear(); }



		bool m_isSorted;
		void clear()
		{
			m_isSorted = false;
			m_vecData.clear();
		}

		void add( KEY key , T val )
		{
			VALUE data;
			data.first = key;
			data.second = val;
			m_vecData.push_back( data );
		}

		T find( KEY key )
		{
			ASSERT( m_isSorted );	
			if ( binary_search( m_vecData.begin() , m_vecData.end() , key , DataCompare() ) )
			{
				CONTAINER::iterator i = lower_bound( m_vecData.begin() , m_vecData.end() , key , DataCompare() );
				if ( i != m_vecData.end() && !(i->first < key ) )
				{
					// 						pair< CONTAINER::iterator , CONTAINER::iterator > range = 
					// 							equal_range( m_vecData.begin() , m_vecData.end() , key , DataCompare() );
					// 
					// 						if ( range.first != range.second )
					// 						{
					// 							return 
					// 						}
					size_t dis = distance( m_vecData.begin() , i );
					return m_vecData[dis].second;
				}
			}
		}

		void sort()
		{				
			std::sort( m_vecData.begin() , m_vecData.end() , DataCompare() );
			m_isSorted = true;
		}
	};

	GdsVectorContainerTest()
	{
	}

	virtual ~GdsVectorContainerTest()
	{
	}

	virtual void SetUp()
	{
		for ( size_t cnt = 0 ; cnt < 100000 ; ++cnt)
		{
			map.insert( pair<int, int>( cnt , cnt ) );
		}
		for ( size_t cnt = 0 ; cnt < 100000 ; ++cnt)
		{
			table.add( cnt , cnt );
		}
		table.sort();	
	}

	virtual void TearDown()
	{
	}

	virtual void TestBody()
 	{
	}

	std::map< int , int > map;
	lookupTable<int , int> table;

};

TEST_F( GdsVectorContainerTest ,  map )
{
	for (size_t cnt = 0 ; cnt < 100000; ++cnt )
	{
		int val = rand()%100000;
		map.find( val );
	}
}

TEST_F( GdsVectorContainerTest , vector )
{
	for (size_t cnt = 0 ; cnt < 100000; ++cnt )
	{
		int val = rand()%100000;
		table.find( val );
	}
}

#endif	