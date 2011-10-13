#include "Precompiled.h"

#include "../Object/Triangle.h"
#include "../Object/3DGrid.h"


#include "TestScene.h"

ImplementRTTI( TestScene, sunSceneBase);


TestScene::TestScene():
m_spCamera(NULL),
m_pGrid(NULL)//,
//m_pTriangle(NULL)
{
}

TestScene::~TestScene()
{

}

int TestScene::InitializeScene()
{
	m_spCamera = new sunCamera;
	assert(m_spCamera);

	m_spCamera->SetName(L"DefaultCamera");
	AppendScene( SmartPointerCast(sunNode, m_spCamera) );
	SetDefaultCamera(m_spCamera);


	m_pGrid = new Grid3D;
	assert(m_pGrid);
	m_pGrid->SetName(L"Grid");
	m_pGrid->CreateNewGrid();
	AppendScene( m_pGrid );

	//m_pTriangle = new Triangle;
	//assert(m_pTriangle);
	//m_pTriangle->CreateTriangle();
	//AppendScene( m_pTriangle);

	sunParserXMLPtr spParserXML = new sunParserXML;
		
	spParserXML->Load(L"boxbody_ani.xml");

	AppendScene( SmartPointerCast(sunNode, spParserXML->GetLoadedEntity()) );

	spParserXML->Destory();
	spParserXML = NULL;
	return true;

}


void TestScene::DestroyScene()
{

	//m_spCamera = NULL;
	
	
	assert(m_pGrid);
	m_pGrid->Destory();
	m_pGrid = NULL;

	//m_pTriangle->Destory();
	//m_pTriangle = NULL;

}


void TestScene::BeginUpdateScene()
{

}

void TestScene::UpdateScene()
{

}

void TestScene::EndUpdateScene()
{

}

void TestScene::AppendScene(sunNodePtr spScene )
{
	assert(spScene);
	sunSceneBase::AppendScene( spScene );
}

void TestScene::PrependScene(sunNodePtr spScene )
{
	assert(spScene);
	sunSceneBase::PrependScene(spScene);
}

void TestScene::RemoveScene( sunNodePtr spScene )
{
	assert( spScene );
	sunSceneBase::RemoveScene( spScene );
}

void TestScene::UpdateSceneList()
{
	sunSceneBase::UpdateSceneList();
}