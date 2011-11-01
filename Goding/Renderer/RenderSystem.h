#ifndef _RENDERER_RENDERSYSTEM9_H_
#define _RENDERER_RENDERSYSTEM9_H_
#pragma once


#ifdef _WINDLL
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif

#include <vector>

#include "RenderObject\IRenderObject.h"
#include "DeviceManager\DeviceManager.h"
#include "..\Common\Camera\Camera.h"


class DLL RenderSystem
{
public:
	RenderSystem(void);
	~RenderSystem(void);

	void					Render();

	IRenderObject*			AllocRenderObject( IRenderObject::ENABLE_RENDER_TYPE type );
	bool					ReleaseRenderObject();

	DeviceManager*			CreateDeviceManager( HWND Window, UINT Width, UINT Height, bool Fullscreen );
	DeviceManager*			GetDeviceManager();

	Camera*					GetCamera(){ return m_pCamera; }
	void					onDeviceLost();
	void					onDeviceReset();

private:

	typedef	std::vector< IRenderObject* > renderObject_table_t;
	renderObject_table_t	m_RenderObjectContainer;

	DeviceManager*			m_pDeviceManager;

	Camera*					m_pCamera;

};

#endif _RENDERER_RENDERSYSTEM9_H_