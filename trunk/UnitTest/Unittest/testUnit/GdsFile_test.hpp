
#pragma once
#include "..\..\..\chodingModule\Snowboard\System\FileSystem\GdsFile.h"
#include "..\..\..\chodingModule\Snowboard\System\FrameMemory\GdsFrameMemory.h"

class GdsFileTest : public ::testing::Test
{
	virtual void SetUp()
	{
		//������
		FRAMEMEMORY.Init( 1024 * 1024 );
		tstring strPath( L"../../Application/Resource/ase/woman.ase" );
		LineContainerA lines( strPath );
		LineContainerA::iterator it = lines.begin();
		LineContainerA::iterator it_end = lines.end();
		++it;
		++it;

	}

	virtual void TearDown()
	{
		//�Ҹ���
	}

	virtual void TestBody()
	{
	}
};

TEST_F( GdsFileTest ,  Module )
{

}