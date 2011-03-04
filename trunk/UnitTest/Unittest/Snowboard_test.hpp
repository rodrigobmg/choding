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
	
	GdsMeshNode*	pMesh1 = new GdsMeshNode;
	GdsNodePtr	   pNode = GdsNodePtr( new GdsNode );

	for ( size_t i = 0 ; i < 10 ; ++i )
	{
		GdsMeshNodePtr pMesh = GdsMeshNodePtr( new GdsMeshNode );
		pNode->AttachChild( pMesh.get() );	
	}
	
	pNode->Update( 0.f );
	
	//pNode->DetachChild( pMesh.get() );
	//ASSERT_TRUE( pNode );
}

#endif	