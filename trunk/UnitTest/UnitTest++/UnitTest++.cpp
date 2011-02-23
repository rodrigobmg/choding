// UnitTest++.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
#include "stdafx.h"
#include "inc/UnitTest++.h"
#pragma comment ( lib , "lib/UnitTest++.vsnet2005.lib" )

#include "../../chodingModule/Snowboard/Framework/Snowboard_stdafx.h"
#pragma  comment ( lib , "../../chodingModule/Lib/Snowboard_D.lib")
#include "../../chodingModule/Snowboard/Core/SceneNode/SceneNode.h"
#include "../../chodingModule/Snowboard/Core/SceneNode/MeshNode.h"


TEST( SceneNodeTest )
{
	SceneNode* pNode = new SceneNode;
// 	for( size_t t = 0 ; t < 1 ; ++t )
// 	{
// 		MeshNode* pChildNode = new MeshNode;
// 		pNode->AttachChild( pChildNode );
// 	}
	MeshNode* pMesh = new MeshNode;
	pNode->AttachChild( pMesh );
	pNode->DetachChild( pMesh );
	pNode->Update( 0.0f );
	CHECK( TRUE );
}

int _tmain(int argc, _TCHAR* argv[])
{
	return UnitTest::RunAllTests();
}

