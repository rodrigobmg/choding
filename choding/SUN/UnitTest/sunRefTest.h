#include "Precompiled.h"

class Add
{

public:
	Add()
	{
		iA = 0;
		iB = 0;
	}
	~Add(){}

public:
	int iA;
	int iB;


};


class RefTest :  public ::testing::Test
{
public:
	RefTest(){}
	~RefTest(){}

	virtual void SetUp()
	{
		pAdd = new Add;

		pAdd2 = pAdd;
	}

	virtual void TearDown()
	{
		delete pAdd;
	}

	Add* pAdd;
	Add* pAdd2;
};

TEST_F(RefTest, test)
{
	pAdd->iA = 1;
	pAdd->iB = 2;

	pAdd2->iA = 3;
	pAdd2->iB = 4;

	//printf("%f", pAdd->iA);
		
	EXPECT_EQ(pAdd->iA, pAdd2->iB);

	//cout << pAdd->iA  << pAdd->iB <<endl;
	
	//cout << pAdd2->iA  << pAdd2->iB <<endl;
}