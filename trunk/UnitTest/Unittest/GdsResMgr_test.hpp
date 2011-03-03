
#ifndef _UNITTEST_GDSRESMGR_H_
#define _UNITTEST_GDSRESMGR_H_

#include "..\..\chodingModule\Snowboard\Core\Resource\GdsResMgr.h"
#include "..\..\chodingModule\Snowboard\Utility\Utility.h"

class GdsResMgrTest : public ::testing::Test
{
public:
	GdsResMgrTest()
	{

	}

	virtual ~GdsResMgrTest()
	{
		
	}

	virtual void SetUp()
	{
		//������
		pResMgr = GdsResMgrPtr( new GdsResMgr );
	}

	virtual void TearDown()
	{
		//�Ҹ���
	}

	virtual void TestBody()
	{
		//�׽�Ʈ�ڵ� ����
		EXPECT_EQ( S_FALSE , pResMgr->LoadRes( L"test" ) );
	}

	GdsResMgrPtr	pResMgr;
};


TEST_F( GdsResMgrTest,  Load )
{
	//GdsResMgrTest resTest;
	GdsResMgrPtr pResMgr = GdsResMgrPtr( new GdsResMgr );
	//ASSERT_TRUE( pResMgr->LoadRes( L"test" ) );
}

#endif	