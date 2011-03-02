#ifndef _UNITTEST_H_
#define _UNITTEST_H_
#include "..\..\chodingModule\Snowboard\Core\SceneNode\GdsNode.h"
#include "..\..\chodingModule\Snowboard\Core\SceneNode\GdsMeshNode.h"

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


TEST_F( NodeTest,  Constructer )
{
	NodeTest test;
	GdsMeshNodePtr pMesh = GdsMeshNodePtr( new GdsMeshNode );
	//GdsMeshNode*	pMesh = new GdsMeshNode;
	GdsNodePtr	   pNode = GdsNodePtr( new GdsNode );

	pNode->AttachChild( pMesh.get() );
	pNode->Update( 0.f );
	
	pNode->DetachChild( pMesh.get() );
	ASSERT_TRUE( pNode );
}

#endif	