#ifndef _UNITTEST_GDSVECTORCONTAINER_H_
#define _UNITTEST_GDSVECTORCONTAINER_H_

#include "..\..\..\chodingModule\Snowboard\System\Util\Utility.h"
#include <hash_map>

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
		for ( size_t cnt = 0 ; cnt < 100 ; ++cnt)
		{
			map.insert( pair<int, tstring >( 100000-cnt , L"map" ) );
			hmap.insert( make_pair( cnt , L"hmap" ) );
			table.add( 100000-cnt , L"vector" );
		}
		table.sort();
	}

	virtual void TearDown()
	{
	}

	virtual void TestBody()
 	{
	}

	std::map< int , tstring > map;
	stdext::hash_map< int , tstring > hmap;
	GdsLookupTable< int , tstring > table;

};

TEST_F( GdsVectorContainerTest , vector )
{
	for (size_t cnt = 0 ; cnt < 100; ++cnt )
	{
		int key = rand()%100;
		table.find( key );
	}
}

TEST_F( GdsVectorContainerTest ,  map )
{
	for (size_t cnt = 0 ; cnt < 100; ++cnt )
	{
		int key = rand()%100;
		map.find( key );
	}
}

TEST_F( GdsVectorContainerTest ,  hash_map )
{
	for (size_t cnt = 0 ; cnt < 100; ++cnt )
	{
		int key = rand()%100;
		hmap.find( key );
	}
}

#endif	