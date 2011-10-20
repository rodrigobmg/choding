#pragma once

#include "..\..\Snowboard\Renderer\GdsRenderObject.h"
#include "..\..\Snowboard\GameObject\GdsGameObject.h"
#include "..\..\Snowboard\Component\ComVisual\GdsComVisualRect.h"

enum { TEST_SIZE = 50000 };
TEST( GdsRenderObjectTest_pool ,  pool_test_renderobject )
{
	GdsRenderObject* prender_object[TEST_SIZE];
	for ( int i=0 ; i < TEST_SIZE ; ++i )
	{
		prender_object[i] = new GdsRenderObject;
// 	}
// 
// 	for ( int i=0 ; i < TEST_SIZE ; ++i )
// 	{
		delete prender_object[i];
	}
}

TEST( GdsGameObjectTest_pool , pool_test_gameobject )
{
	GdsGameObject* pgame_object[TEST_SIZE];
	for ( int i=0 ; i < TEST_SIZE ; ++i )
	{
		pgame_object[i] = new GdsGameObject;
//	}
//
// 	for ( int i=0 ; i < TEST_SIZE ; ++i )
// 	{
		delete pgame_object[i];
	}
}

TEST( GdsComVisualRectTest_pool , pool_test_comvisual )
{
	GdsComVisualRect* p_com_visual_rect[TEST_SIZE];
	for ( int i=0 ; i < TEST_SIZE ; ++i )
	{
		p_com_visual_rect[i] = new GdsComVisualRect;
// 	}
// 
// 	for ( int i=0 ; i < TEST_SIZE ; ++i )
// 	{
		delete p_com_visual_rect[i];
	}
}

