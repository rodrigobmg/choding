
#pragma once

#include "..\..\..\chodingModule\Snowboard\Core\Terrain\GdsTerrain.h"
#include "..\..\..\chodingModule\Snowboard\Core\Terrain\GdsTerrain_Triangle.h"

class GdsTerrainTest : public ::testing::Test
{
public:
	GdsTerrainTest()
	{
	}

	virtual ~GdsTerrainTest()
	{
	}

	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{
		//¼Ò¸êÀÚ
	}

	virtual void TestBody()
	{		
	}
};



TEST_F( GdsTerrainTest,  MakeHeightMap )
{	 
	D3DXVECTOR3 pos( 32 , 0 ,32 );
	WORD index;
	TRIANGLE triangle;
	triangle.GetIndex( pos , index );
	EXPECT_EQ( 33*33-1 , (int)index );
	TERRAIN.MakeHeightMap();		
}

TEST_F( GdsTerrainTest , Update )
{
	//TERRAIN.Update(0.f);
}
