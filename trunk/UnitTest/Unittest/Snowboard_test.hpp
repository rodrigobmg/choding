#ifndef _UNITTEST_H_
#define _UNITTEST_H_
#include "..\..\chodingModule\Snowboard\Core\SceneNode\GdsNode.h"
#include "..\..\chodingModule\Snowboard\Core\SceneNode\GdsMeshNode.h"

class NodeTest : public ::testing::Test
{
public:
	NodeTest()
	{
		SetUp();
		TestBody();
	}

	virtual ~NodeTest()
	{
		TearDown();
	}

	virtual void SetUp()
	{
		//持失切
		pNode = GdsNodePtr( new GdsNode );
	}

	virtual void TearDown()
	{
		//社瑚切
	}

	virtual void TestBody()
	{
		for ( size_t i = 0 ; i < 2 ; ++i )
		{
			GdsMeshNodePtr pMesh = GdsMeshNodePtr( new GdsMeshNode );
			pNode->AttachChild( pMesh.get() );	
		}
		pNode->Update( 0.f );
	}

	GdsNodePtr	   pNode;
};

TEST_F( NodeTest,  TestBody )
{		
	NodeTest test;
}

#endif	