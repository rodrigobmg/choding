#ifndef _SUN_UNITTEST_SUNVECTORTEST_H_
#define _SUN_UNITTEST_SUNVECTORTEST_H_


#include "Precompiled.h"

//////////////////////////////////////////////////////////////////////////
// 얘약한 벡터와 예약안한 벡터의 성능 테스트
// 생각만큼 큰 차이는 없다. 하지만 reserve 한 벡터가 속도는 조금 더 빠르다.
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