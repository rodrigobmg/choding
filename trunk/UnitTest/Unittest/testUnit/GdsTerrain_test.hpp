
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
	size_t buffersize = 10;
	D3DXVECTOR3* vBuffer = new D3DXVECTOR3[buffersize];
	for ( size_t i=0 ; i < buffersize ; i++)
	{
		vBuffer[i] = D3DXVECTOR3( rand()%10 , rand()%10 , rand()%10 );
	}	

	//분할할 볼륨을 설정해줌 세번째 인자는 부모의 포인터
	GdsOctree test( vBuffer , buffersize , 3 );
	test.Build();
}
