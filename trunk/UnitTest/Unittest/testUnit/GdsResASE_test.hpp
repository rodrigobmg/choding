#ifndef _UNITTEST_GDSRESASE_H_
#define _UNITTEST_GDSRESASE_H_

#include "..\..\..\chodingModule\Snowboard\Core\Resource\Type\GdsResASE.h"
#include "..\..\..\chodingModule\Snowboard\System\FrameMemory\GdsFrameMemory.h"

class GdsResASETest : public ::testing::Test
{
public:
	GdsResASETest()
	{
	}

	virtual ~GdsResASETest()
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

TEST_F( GdsResASETest ,  GdsResASE )
{		
//	FRAMEMEMORY.Init( 1024*1024 );
	GdsResASEPtr res = GdsResASEPtr( new GdsResASE );

	res->Create( L"../../Application/Resource/ASE/woman.ASE" , NULL );
}

#endif	