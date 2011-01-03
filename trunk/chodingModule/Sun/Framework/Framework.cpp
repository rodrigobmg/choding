#include "Precompiled.h"
#include "Framework.h"

Sun_CCoreManager*			g_pCoreManager		= NULL;

//초기화는 생성자에서 해야함.
Sun_CFramewrok::Sun_CFramewrok():m_hWnd(NULL),m_bExitFlag(false),m_pGraphicCore(NULL)
{
}

Sun_CFramewrok::~Sun_CFramewrok()
{
}


void Sun_CFramewrok::ShutDown()
{
	Safe_Delete(g_pCoreManager);
}


void Sun_CFramewrok::Run( HWND hWnd)
{
	assert( hWnd );
	m_hWnd = hWnd;


	if( CreateCoreManager() )
	{
		if(OnInitialize())
		{
			ProcLoop();
		}
	}

	ShutDown();
}

void Sun_CFramewrok::ProcLoop()
{
	MSG msg;
	ZeroMemory( &msg, sizeof(msg));
	while( WM_QUIT != msg.message)
	{
		//메시지큐에 메시지가 있으면 메시지 처리
		if( PeekMessage (&msg, NULL, 0U, 0U ,PM_REMOVE))
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			if( m_bExitFlag )
				break;

		}

	}

}

bool Sun_CFramewrok::CreateCoreManager()
{
	assert( NULL == g_pCoreManager);

	//CoreManager를 생성
	g_pCoreManager = new Sun_CCoreManager(m_hWnd);
	

	//이곳에서 코어 생성 inputCore, GraphicCore, ResourceCore, SceneCore
	m_pGraphicCore = dynamic_cast<Sun_CGraphicCore*>(g_pCoreManager->RegisterCore( CORENAME_GRAPHIC, new Sun_CGraphicCore ));

	return true;
}



bool Sun_CFramewrok::MessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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


