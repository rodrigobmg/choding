#pragma once


//#include "Window.h"
#include "..\Common\Type\type.h"

//Function pointers
typedef void (*ON_DEVICE_LOST)();
typedef void (*ON_DEVICE_RESET)();

//PerfHud enable?
#define PERF

//Unlocked Framerate?
#define FREE

class DeviceManager
{
private:
	//Context
	IDirect3D9* Context;

	//Device
	IDirect3DDevice9* Device;

	//BackBuffer
	IDirect3DSurface9* BackBuffer;

	//AutoDepthBuffer
	IDirect3DSurface9* DepthBuffer;

	//TempVertex Declarations
	IDirect3DVertexDeclaration9* MeshVD;
	IDirect3DVertexDeclaration9* SkinnedMeshVD;
	IDirect3DVertexDeclaration9* MorphingMeshVD;

	//Synchronising Query
	IDirect3DQuery9* Query;

	//Presentation Parameters
	D3DPRESENT_PARAMETERS Parameters;

public:
	//Pointer to onDeviceLost/onDeviceReset functions outside of this class
	ON_DEVICE_LOST OnDeviceLost;
	ON_DEVICE_RESET OnDeviceReset;

	IDirect3D9* getContext() { return Context; }
	IDirect3DDevice9* getDevice() { return Device; }
	IDirect3DSurface9* getBackBuffer() { return BackBuffer; }
	IDirect3DSurface9* getDepthBuffer() { return DepthBuffer; }
	IDirect3DQuery9* getQuery() { return Query; }
	D3DPRESENT_PARAMETERS getPresentationParameters() { return Parameters; }

	void setBackBuffer() { Device->SetRenderTarget(0, BackBuffer); }
	void setAutoDepthStencil() { Device->SetDepthStencilSurface(DepthBuffer); }

	void setMeshDeclaration() { Device->SetVertexDeclaration(MeshVD); }
	void setSkinnedMeshDeclaration() { Device->SetVertexDeclaration(SkinnedMeshVD); }
	void setMorphingMeshDeclaration() { Device->SetVertexDeclaration(MorphingMeshVD); }

	//Constructor
	DeviceManager(HWND Window, UINT Width, UINT Height, bool Fullscreen);
	~DeviceManager();

	//OnDeviceLost
	void onDeviceLost();
	//OnDeviceReset
	void onDeviceReset();

	//Change view mode
	void changeViewMode( int Width, int Height, bool Fullscreen);

	//Error check a DirectX thrown HRESULT
	void errorCheck(HRESULT result, LPCTSTR debugInfo);

	//Synchronising function
	void Present();
};
