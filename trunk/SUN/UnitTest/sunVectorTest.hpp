#ifndef _SUN_UNITTEST_SUNVECTORTEST_H_
#define _SUN_UNITTEST_SUNVECTORTEST_H_


#include "Precompiled.h"

//////////////////////////////////////////////////////////////////////////
// ����� ���Ϳ� ������� ������ ���� �׽�Ʈ
// ������ŭ ū ���̴� ����. ������ reserve �� ���Ͱ� �ӵ��� ���� �� ������.
//////////////////////////////////////////////////////////////////////////

const int iTestSize = 1000000; 

typedef struct _stSizeClass
{
	int		iTemp[100];
	float	fTemp[100];
}stSizeClass;



class sunVectorTest : public ::testing::Test
{
public:

	sunVectorTest(){}

	virtual ~sunVectorTest(){}


	virtual void SetUp()
	{
		NotReservevec.clear();

		ReserveVec.clear();
		ReserveVec.reserve(iTestSize);
	}
	
	virtual void TearDown()
	{

	}

	//virtual void TestBody()
	//{

	//}

	std::vector<int> NotReservevec;
	std::vector<int> ReserveVec;

};


TEST_F( sunVectorTest, NotReservevec)
{
	for(int i=0; i < iTestSize; ++i)
	{
		//stSizeClass kSizeClass;

		NotReservevec.push_back(i);	
	}
}

TEST_F( sunVectorTest, ReserveVec)
{
	for(int i=0; i < iTestSize; ++i)
	{
		//stSizeClass kSizeClass;
		ReserveVec.push_back(i);	
	}
}

#endif