#pragma once

typedef void (*ON_DEVICE_LOST)();
typedef void (*ON_DEVICE_RESET)();

//PerfHud enable?
#define PERF

//Unlocked Framerate?
#define FREE

#ifdef _WINDLL
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif

class DLL DeviceManager
{
private:

	IDirect3D9* Context;
	IDirect3DDevice9* Device;
	IDirect3DSurface9* BackBuffer;
	IDirect3DSurface9* DepthBuffer;
	IDirect3DVertexDeclaration9* MeshVD;
	IDirect3DQuery9* Query;
	D3DPRESENT_PARAMETERS Parameters;

public:
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

	DeviceManager(HWND Window, UINT Width, UINT Height, bool Fullscreen);
	~DeviceManager();

	void onDeviceLost();
	void onDeviceReset();

	void changeViewMode( int Width, int Height, bool Fullscreen);

	void errorCheck(HRESULT result, LPCTSTR debugInfo);

	void Present();
};
