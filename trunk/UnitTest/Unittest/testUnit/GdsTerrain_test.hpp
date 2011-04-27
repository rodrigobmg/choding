
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
	size_t buffersize = 10;
	D3DXVECTOR3* vBuffer = new D3DXVECTOR3[buffersize];
	for ( size_t i=0 ; i < buffersize ; i++)
	{
		vBuffer[i] = D3DXVECTOR3( rand()%10 , rand()%10 , rand()%10 );
	}	

	//������ ������ �������� ����° ���ڴ� �θ��� ������
	GdsOctree test( vBuffer , buffersize , 3 );
	test.Build();
}
