#pragma once

#include <iostream>
#include <vector>
#include <windows.h>

#include "Win_Window.h"
#include "..\..\Renderer\RenderSystem.h"
#include "..\ColladaLoader\ColladaStaticMesh.h"

using namespace std;

namespace WinApplication
{
	//Function definitions
	LRESULT CALLBACK messageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void Clean();
	void onDeviceLost();
	void onDeviceReset();
	void Update();
	void Draw();

	//Base
	Window* window;
	//Time* time;

	//Window focus
	bool inputFocus;

	//Screen params
	float Width;
	float Height;
	bool Fullscreen;
	
	EntitySystem* m_pEntitySystem;
	RenderSystem* m_pRenderSystem;

	//Main
	void Run(EntitySystem* pEntitySystem , RenderSystem* pRenderSystem , float width, float height, bool fullscreen)
	{
		m_pEntitySystem = pEntitySystem;
		m_pRenderSystem = pRenderSystem;
		
		Width = width; Height = height; Fullscreen = fullscreen;

		window = new Window(WinApplication::messageHandler, L"Goding", 0, 0, (int)Width, (int)Height);


		ColladaStaticMesh loader;
		std::string str("fhi");
		loader.Load( str  , pEntitySystem );

		pRenderSystem->CreateDeviceManager( window->getHandle(), (int)Width, (int)Height, Fullscreen );
		pRenderSystem->GetDeviceManager()->OnDeviceLost = onDeviceLost;
		pRenderSystem->GetDeviceManager()->OnDeviceReset = onDeviceReset;
		
		window->Show();

		ShowCursor(false);

		inputFocus = true;

		MSG msg;
		ZeroMemory(&msg, sizeof(msg));
		
		//Main loop
		SetCursorPos(600, 600);
		while(msg.message != WM_QUIT)
		{
			if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				//Check for the active window focus
 				bool windowFocus = (window->getHandle() == GetActiveWindow());
 
 				//If the window has the full attention, input focus and active focus as well as device focus
 				if(inputFocus && windowFocus && (pRenderSystem->GetDeviceManager()->getDevice()->TestCooperativeLevel() == D3D_OK))
 				{
 					//Then Update and draw
 					Update();
 					Draw();
 				}
 				else if(windowFocus && (pRenderSystem->GetDeviceManager()->getDevice()->TestCooperativeLevel() != D3D_OK)) //If window focus is regained but the device is lost
 				{
 					//If the device is recoverable then recover
 					if(pRenderSystem->GetDeviceManager()->getDevice()->TestCooperativeLevel() == D3DERR_DEVICELOST  
 						|| pRenderSystem->GetDeviceManager()->getDevice()->TestCooperativeLevel() == D3DERR_DEVICENOTRESET
						) 
 					{
						//pRenderSystem->GetDeviceManager()->changeViewMode( (int)Width, (int)Height, Fullscreen );
						if(!Fullscreen) window->setSize( (int)Width, (int)Height );
						if(Fullscreen)
							while(ShowCursor(false) >= 0);
						else
							while(ShowCursor(true) < 0);
						ShowCursor(false);
					}
 
 					//If theres a driver error then say goodbye
 					if(pRenderSystem->GetDeviceManager()->getDevice()->TestCooperativeLevel() == D3DERR_DRIVERINTERNALERROR)
 					{
 						MessageBox(NULL, TEXT("Driver Internal Error"), TEXT("Error"), MB_OK | MB_ICONERROR);
 
 						break;
 					}
 				}
			}
		}

		Clean();
	}

	//Clean Application
	void Clean()
	{
		SAFE_DELETE( window );
	}

	//Handle device lost event
	void onDeviceLost()
	{
		//Handle meshes
		m_pRenderSystem->onDeviceLost();
	}

	//Handle device reset event
	void onDeviceReset()
	{
		//Handle meshes
	//	m_pRenderSystem->onDeviceReset(m_pRenderSystem->getDevice());

		//Handle camera
		//camera->onDeviceReset(Width, Height);
	}

	//Update Application
	void Update()
	{
		//Get Escape key state
		bool Escape = (GetAsyncKeyState(VK_ESCAPE) & 0x8000) ? true : false;

		//Quit if pressed
		if(Escape) PostQuitMessage(0);

		//Update Time
		//time->Update();

		//Update Camera
		//camera->Update(60.0f / time->getFps());

		m_pEntitySystem->Update( 0.f );
	}

	//Draw Meshes
	void Draw()
	{
 		m_pRenderSystem->GetDeviceManager()->getDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
 
 		m_pRenderSystem->GetDeviceManager()->getDevice()->BeginScene();
 
 		//pRenderSystem->Draw(deviceManager, camera);
 
 		m_pRenderSystem->GetDeviceManager()->getDevice()->EndScene();
 
 		//m_pRenderSystem->GetDeviceManager()->Present();
	}

	//Message handler
	LRESULT CALLBACK messageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch(msg)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;
		case WM_SETFOCUS:
			inputFocus = true;
			return 0;
		case WM_KILLFOCUS:
			inputFocus = false;
			break;
		case WM_SYSCOMMAND:
			switch(wParam)
			{
			case SC_CLOSE:
				PostQuitMessage(0);
				return 0;
			case SC_MINIMIZE:
				break;
			case SC_MOVE:
				break;
			case SC_RESTORE:
				break;
			case SC_MAXIMIZE:
				break;
			default:
				return 0;
			}
			break;
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}
