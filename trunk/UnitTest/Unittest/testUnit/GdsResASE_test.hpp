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
		//持失切
	}

	virtual void TearDown()
	{
		//社瑚切
	}

	virtual void TestBody()
	{	
	}
};

TEST_F( GdsResASETest ,  GdsResASE )
{		
	FRAMEMEMORY.Init( 1024*1024 );
	GdsResASEPtr res = GdsResASEPtr( new GdsResASE );

	tstring strpath = L"../../Application/Resource/ASE/woman.ASE";
	res->Create( strpath.c_str() , NULL );
}

#endif	