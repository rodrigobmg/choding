// UnitTest++.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
#include "stdafx.h"
#include "inc/UnitTest++.h"
#pragma comment ( lib , "lib/UnitTest++.vsnet2005.lib" )

#include "../../chodingModule/Snowboard/Framework/Snowboard_stdafx.h"
#include "../../chodingModule/Snowboard/Utility/Log/logger.h"
#include "../../chodingModule/Snowboard/Utility/ThreadPool/ThreadPool.h"
#pragma  comment ( lib , "../../chodingModule/Lib/Snowboard_D.lib")


TEST( hellounittest )
{
	util::Logger::createSingleton();
	util::Logger::getInstance().Init(NULL , NULL , NULL , NULL );

	ThreadPool::createSingleton();
	ThreadPool::getInstance().Create( 10 );

	// �����ѵ� �̵� �ڵ� �׽�Ʈ�� �ӷ� �����ϳ�..�Ѥ�;; ���� �ڵ� ������ �׽�Ʈ�ϱ� ���� ¥���� �ٲٴ°� ���� �ʿ��ҵ� ������
// 	LOG_WARNING_F( L"%s", L"logger init" );
// 	TCHAR curpath[ MAX_PATH ];
// 	GetCurrentDirectory( MAX_PATH, curpath );
// 	TCHAR respath[MAX_PATH];
// 	_stprintf_s( respath , L"%s\\%s" , L"D:\\Project\\Client\\Trunk\\WorkGroup\\Client\\Application" , L"" );
// 
// 	if ( m_pResMgr )
// 	{		
// 		SAMPLE_PERFORMANCE loadsample;
// 		BEGIN_PERFORMANCE( L"list" );
// 		if ( m_pResMgr->CreateList( L"test" , respath , L"tga;bmp;dds" , 1 ) )
// 		{
// 			END_PERFORMANCE( L"list" );
// 			ThreadPool::getInstance().GetIdleThread()->Push< const TCHAR* >( m_pResMgr , L"test"  , &CResMrg::LoadRes );
// 		}
// 
// 		OUTPUT_PERFORMANCE( L"list" , loadsample );
// 		LOG_ERROR_F( "list avg tick = %d" , loadsample.ulAvg );
// 
// 		CResTexture* p = static_cast< CResTexture*>( m_pResMgr->Get( L"test" , L"banana.bmp" ) );
// 		//�Ҵ��� ������ �� �������ؼ� ��ȯ�� �Ѵ�.
// 		if ( p )
// 			p->Release();		
// 	}

	CHECK( ThreadPool::getInstance().GetIdleThread() );
}

int _tmain(int argc, _TCHAR* argv[])
{
	return UnitTest::RunAllTests();
}

