#ifndef _UNITTEST_H_
#define _UNITTEST_H_

class SnowboardTest : public ::testing::Test
{
public:
	SnowboardTest()
	{
	}

	virtual ~SnowboardTest()
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

TEST_F( SnowboardTest,  inittest )
{		
}

#endif	