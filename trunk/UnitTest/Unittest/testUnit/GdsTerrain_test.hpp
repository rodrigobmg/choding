
#pragma once

#include "..\..\..\chodingModule\Snowboard\Core\Terrain\GdsTerrain.h"

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
	TERRAIN.MakeHeightMap();		
}
