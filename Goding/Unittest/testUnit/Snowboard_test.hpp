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

TEST_F( SnowboardTest,  inittest )
{		
}

#endif	