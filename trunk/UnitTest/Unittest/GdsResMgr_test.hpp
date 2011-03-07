
#ifndef _UNITTEST_GDSRESMGR_H_
#define _UNITTEST_GDSRESMGR_H_

#include "..\..\chodingModule\Snowboard\Utility\Utility.h"
#include "..\..\chodingModule\Snowboard\Core\Camera\GdsCameraManagerDX9.h"


class GdsCameraManagerDX9Test : public ::testing::Test
{
public:
	GdsCameraManagerDX9Test()
	{
		cammgr = GdsCameraManagerDX9Ptr( new GdsCameraManagerDX9 );
	}

	virtual ~GdsCameraManagerDX9Test()
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
		//테스트코드 ㅋㅋ
		GdsCameraNodePtr camnode	= GdsCameraNodePtr( new GdsCameraNode );
		cammgr->Attach( camnode );
		int index = 0;	
		cammgr->SetCam( index );
		cammgr->Update( 0.0f );
		cammgr->Detach( index );
	}

	GdsCameraManagerDX9Ptr	cammgr;
};


TEST_F( GdsCameraManagerDX9Test ,  Module )
{
	GdsCameraManagerDX9Test test;
	test.TestBody();
}

#endif	