
#pragma once

#include "..\..\..\chodingModule\Snowboard\Core\SceneNode\GdsNode.h"
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
		//�Ҹ���
	}

	virtual void TestBody()
	{		
	}
};

 TEST_F( GdsTerrainTest,  MakeHeightMap )
 {	 
	 GdsNodePtr pNode = GdsNodePtr( new GdsNode );
	 TERRAIN.MakeHeightMap();		
 }
