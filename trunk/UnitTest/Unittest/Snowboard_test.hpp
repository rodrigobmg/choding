#ifndef _UNITTEST_H_
#define _UNITTEST_H_
#include "..\..\chodingModule\Snowboard\Core\SceneNode\GdsNode.h"

namespace ChodingTestGdsNode
{
	class NodeTest : public ::testing::Test
	{
	public:
		NodeTest()
		{

		}

		virtual ~NodeTest()
		{
			
		}

		virtual void SetUp()
		{
			//생성자
		}

		virtual void TearDown()
		{
			//소멸자
		}

		virtual void TestBody()
		{
			//테스트코드 ㅋㅋ
		}
	};
}


TEST( NodeTest,  Constructer )
{
	ChodingTestGdsNode::NodeTest test;
	ASSERT_TRUE( &test );
}

#endif	