// UnitTest++.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "inc/UnitTest++.h"
#pragma comment ( lib , "lib/UnitTest++.vsnet2005.lib" )

TEST( hellounittest )
{
	CHECK( false );
}

int _tmain(int argc, _TCHAR* argv[])
{
	return UnitTest::RunAllTests();
}

