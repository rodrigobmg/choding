
#ifndef _UNITTEST_SUNFILEMEMORY_H_
#define _UNITTEST_SUNFILEMEMORY_H_

#include "..\..\..\chodingModule\Snowboard\System\FrameMemory\GdsFrameMemory.h"
#include "..\..\..\chodingModule\Snowboard\System\Logger\logger.h"
#include "..\..\..\chodingModule\Snowboard\System\FileSystem\GdsFile.h"

class SunFrameMemoryTest : public ::testing::Test
{
public:
	SunFrameMemoryTest()
	{
	}

	virtual ~SunFrameMemoryTest()
	{
	}

	virtual void SetUp()
	{

		//const TCHAR* token = text.Next();
	}

	virtual void TearDown()
	{
		//소멸자
	}

	virtual void TestBody()
	{	
	}
};

//메모리 한번에 로드하고 읽기
TEST_F( SunFrameMemoryTest ,  GdsFrameMemory )
{
	FRAMEMEMORY.Init( 1024*1024 );

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
}


//한줄 한줄 읽기
TEST_F(SunFrameMemoryTest, LineMemory)
{
	FILE *fp;
	fp = fopen("../../Application/Resource/ASE/woman.ASE", "rt");
	EXPECT_TRUE(fp);
	
	char m_line[256] = {0,};

	while(!feof(fp))
	{
		fgets(m_line, 256, fp);
	}

	fclose(fp);
}


#endif	