
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
		//������

	}

	virtual void TearDown()
	{
		//�Ҹ���
	}

	virtual void TestBody()
	{
	
	}
};

TEST_F( GdsOctreeTest,  inittest )
{		
	D3DXVECTOR3 minVec( 0 , 0 , 0 );
	D3DXVECTOR3 maxVec( 10 , 10 , 10 );

	//������ ������ �������� ����° ���ڴ� �θ��� ������
	GdsOctree test( minVec , maxVec , 3 );
	test.Build();
}
