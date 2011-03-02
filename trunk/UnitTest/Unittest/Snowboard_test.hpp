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