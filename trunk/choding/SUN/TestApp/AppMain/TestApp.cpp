#include "Precompiled.h"
#include "TestApp.h"

#include "../Object/3DGrid.h"
#include "../Scene/TestScene.h"

sunCoreManager* g_pCoreManager = NULL;

TestApp::TestApp()
{


}

TestApp::~TestApp()
{

}

LRESULT CALLBACK TestApp::MsgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{

	switch( message )
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc( hWnd, message, wParam, lParam);

}

bool TestApp::CreateCoreManager()
{

	g_pCoreManager = new sunCoreManager(m_hWnd);
	assert(g_pCoreManager);
	g_pCoreManager->InitTimeLine();

	m_pGraphicCore = (sunGraphicCore*)g_pCoreManager->RegisterCore( CORENAME_GRAPHIC, new sunGraphicCore );
	m_pSceneCore   = (sunSceneCore*)g_pCoreManager->RegisterCore( CORENAME_SCENE, new sunSceneCore );

	assert(m_pGraphicCore);
	assert(m_pSceneCore);

	return true;
}

bool TestApp::OnInitialize()
{
	assert(m_pGraphicCore);
	assert(m_pSceneCore);

	sunSceneBase* pScene = NULL;
	pScene = new TestScene;

	assert(pScene);

	m_pSceneCore->SetNextScene( pScene );

	return true;
}



bool TestApp::Start(HWND hWnd)
{
	m_hWnd = hWnd;

	//이곳에서 엔진 생성 클라 초기화
	
	CreateCoreManager();
	OnInitialize();

	//Loop처리.
	MsgLoop();

	return true;
}



void TestApp::End()
{
	assert(m_pGraphicCore);
	g_pCoreManager->DestroyCore();
	
	Safe_Delete(g_pCoreManager);
}

void TestApp::MsgLoop()
{
	while(true)
	{
		MSG msg;
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			if(msg.message == WM_QUIT)
				break;

			if( !TranslateAccelerator( m_hWnd, NULL, &msg ) )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
		else
		{
			g_pCoreManager->CalculateElapsedTimeFPS();

			m_pSceneCore->UpdateCore();
			m_pGraphicCore->UpdateCore();
		}
	}


}