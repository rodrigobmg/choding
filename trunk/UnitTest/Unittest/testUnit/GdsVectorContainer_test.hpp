#ifndef _UNITTEST_GDSVECTORCONTAINER_H_
#define _UNITTEST_GDSVECTORCONTAINER_H_

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

class GdsVectorContainerTest : public ::testing::Test
{
public:
	GdsVectorContainerTest()
	{
	}

	virtual ~GdsVectorContainerTest()
	{
	}

	virtual void SetUp()
	{
// 		vector<int> vi;
// 		for ( size_t t = 0 ; t < 10 ; t++ )
// 		{
// 			int temp_value = 10-t;
// 			vi.push_back( temp_value );
// 		}
// 		sort( vi.begin() , vi.end() );
// 		if ( binary_search( vi.begin() , vi.end() , 5 ) )
// 		{
// 			vector<int>::iterator i = lower_bound( vi.begin() , vi.end() , 5 );
// 			if ( i != vi.end() && !(*i<5) )
// 			{
// 				pair< vector<int>::iterator , vector<int>::iterator > range = equal_range( vi.begin() , vi.end() , 5 );
// 				if( range.first != range.second )
// 				{
// 					if ( distance( range.first , range.second ) == 1 )
// 					{
// 						*(i) = 7;
// 					}							
// 				}
// 			}
// 		}
// 
// 		sort( vi.begin() , vi.end() );		
		lookupTable< int , wchar_t* > table;
		for ( size_t t = 0 ; t < 10 ; t++ )
		{
			size_t val = 10 - t;
			wchar_t* temp = L"hi";
			table.add( (int)val , temp );
		}
		table.sort();
		wchar_t* rval = table.find( 2 );		
	}

	virtual void TearDown()
	{
	}

	virtual void TestBody()
	{	
	}
};

TEST_F( GdsVectorContainerTest ,  GdsVectorContainer )
{		
}

#endif	