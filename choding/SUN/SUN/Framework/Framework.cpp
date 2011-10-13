#include "Precompiled.h"
#include "Framework.h"

sunCoreManager*	g_pCoreManager = NULL;


sunFramework::sunFramework():
m_hWnd(NULL),
m_bExitFlag(FALSE),
m_pGraphicCore(NULL),
m_pSceneCore(NULL)
{

}


sunFramework::~sunFramework()
{
}


void sunFramework::ShutDown()
{
	Safe_Delete(g_pCoreManager);

}

bool sunFramework::CreateCoreManager()
{
	assert( NULL == g_pCoreManager );

	g_pCoreManager = new sunCoreManager( m_hWnd );		
	assert(g_pCoreManager);



	m_pGraphicCore = static_cast<sunGraphicCore*>(g_pCoreManager->RegisterCore( CORENAME_GRAPHIC, new sunGraphicCore));
	m_pSceneCore   = static_cast<sunSceneCore*>(g_pCoreManager->RegisterCore(CORENAME_SCENE, new sunSceneCore));


	assert( NULL != m_pGraphicCore );
	assert( NULL != m_pSceneCore );

	return true;
}

void sunFramework::Run(HWND hWnd)
{
	assert( hWnd );
	m_hWnd = hWnd;

	//코어 매니져 생성
	if(true == CreateCoreManager())
	{
		//초기화 할꺼 초기화 하고.
		if(OnInitialize())
		{
			ProcLoop();
		}

	}


	ShutDown();

}


void sunFramework::ProcLoop()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while( WM_QUIT != msg.message)
	{
	
		if( PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			if(true == m_bExitFlag)
				break;

			//메인 루프 업데이트 순서
			//Scene -> Render

			m_pSceneCore->UpdateCore();
			m_pGraphicCore->UpdateCore();

		}

	}

}

bool sunFramework::MessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	
	switch(uMsg)
	{
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return false;
		}
	case WM_KEYUP:
		{
			switch( wParam )
			{
			case VK_ESCAPE:
				{
					m_bExitFlag = true;
					break;
				}

			}
		}
	}

	return true;

}