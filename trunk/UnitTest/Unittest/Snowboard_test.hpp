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
			//������
		}

		virtual void TearDown()
		{
			//�Ҹ���
		}

		virtual void TestBody()
		{
			//�׽�Ʈ�ڵ� ����
		}
	};
}


TEST( NodeTest,  Constructer )
{
	ChodingTestGdsNode::NodeTest test;
	ASSERT_TRUE( &test );
}

#endif	