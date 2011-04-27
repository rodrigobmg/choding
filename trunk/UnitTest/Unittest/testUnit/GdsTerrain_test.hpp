
#pragma once

#include "..\..\..\chodingModule\Snowboard\Core\SceneNode\Octree\GdsOctree.h"

class GdsOctreeTest : public ::testing::Test
{
public:
	GdsOctreeTest()
	{
	}

	virtual ~GdsOctreeTest()
	{
	}

	virtual void SetUp()
	{
		//생성자

	}

	virtual void TearDown()
	{
		//소멸자
	}

	virtual void TestBody()
	{
	
	}
};

TEST_F( GdsOctreeTest,  inittest )
{		
	D3DXVECTOR3 minVec( 0 , 0 , 0 );
	D3DXVECTOR3 maxVec( 10 , 10 , 10 );

	//분할할 볼륨을 설정해줌 세번째 인자는 부모의 포인터
	GdsOctree test( minVec , maxVec , 3 );
	test.Build();
}
