// UnitTest++.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
#include "stdafx.h"
#include "inc/UnitTest++.h"
#pragma comment ( lib , "lib/UnitTest++.vsnet2005.lib" )
#pragma  comment ( lib , "../../chodingModule/Lib/Snowboard_D.lib")

#include "../../chodingModule/Snowboard/Core/Resource/Type/GdsResTexture.h"


TEST( shared_ptr_test )
{
	//GdsNodePtr pNode = boost::shared_ptr< GdsNode >( (GdsNode*)NULL );
	GdsResTexturePtr ptex = GdsResTexturePtr( new GdsResBase );
	//GdsResBasePtr pbase = GdsResBasePtr( new GdsResTexture );
	CHECK( TRUE );
}

int _tmain(int argc, _TCHAR* argv[])
{
	return UnitTest::RunAllTests();
}

