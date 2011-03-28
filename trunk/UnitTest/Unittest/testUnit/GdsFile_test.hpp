
#ifndef _UNITTEST_GDSFILE_H_
#define _UNITTEST_GDSFILE_H_

#include "..\..\..\chodingModule\Snowboard\System\FrameMemory\GdsFrameMemory.h"
#include "..\..\..\chodingModule\Snowboard\Utility\Log\logger.h"
#include "..\..\..\chodingModule\Snowboard\System\FileSystem\GdsFile.h"

class GdsFrameMemoryTest : public ::testing::Test
{
public:
	GdsFrameMemoryTest()
	{
	}

	virtual ~GdsFrameMemoryTest()
	{
	}

	virtual void SetUp()
	{
		//持失切
		FRAMEMEMORY.Init( 1024*1024 );
		LOG_WARNING_F( "FrameMemory Init %d Byte" , FRAMEMEMORY.GetSize() );
		BYTE* p = (BYTE*)FRAMEMEMORY.Alloc( 512 );
		_stprintf_s( (wchar_t*)p , 512, L"hello" );
		BYTE* p2 = (BYTE*)FRAMEMEMORY.Alloc( 200 );
		ASSERT_TRUE( p2 );
		FRAMEMEMORY.Free( 200, p2);
		FRAMEMEMORY.Free( 512, p );

		//ReadFileInMemory text( tstring( L"../../Application/Resource/ASE/woman.ASE" ) , "\0" );
		//ReadFileInMemory text( tstring( L"../../Application/Resource/ASE/test.txt" ) , L"\0" );
		tstring strpath = L"../../Application/Resource/ASE/woman.ASE";
		LineContainerA lines( strpath );
		EXPECT_FALSE( lines.empty() );
		LineContainerA::iterator it = lines.begin();
		LineContainerA::iterator it_end	= lines.end();
		++it;
		for ( ; it != it_end ; ++it )
		{
			if ( it == it_end )
				break;
		}

		//const TCHAR* token = text.Next();
	}

	virtual void TearDown()
	{
		//社瑚切
	}

	virtual void TestBody()
	{	
	}
};

TEST_F( GdsFrameMemoryTest ,  GdsFrameMemory )
{		
}

#endif	