#include "StdAfx.h"
#include "DeviceManager.h"


DeviceManager::DeviceManager(HWND Window, UINT Width, UINT Height, bool Fullscreen)
{

	//Set all pointers to NULL in one line
	Context = NULL; 
	Device = NULL; 
	BackBuffer = NULL; 
	DepthBuffer = NULL; 
	MeshVD = NULL; 
	Query = NULL; 
	OnDeviceLost = NULL; 
	OnDeviceReset = NULL;

	//Initialize Context
	Context = Direct3DCreate9(D3D_SDK_VERSION);

	//Check if Context initialization was successful
	if(!Context) throw("Could not create Direct3D context!\n");

#pragma region //Set up Presentation Parameters
	Parameters.BackBufferWidth = Width;
	Parameters.BackBufferHeight = Height;
	Parameters.BackBufferFormat = D3DFMT_A8R8G8B8;
	Parameters.BackBufferCount = 1;

	Parameters.MultiSampleType = D3DMULTISAMPLE_NONE;
	Parameters.MultiSampleQuality = 0;

	Parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	Parameters.hDeviceWindow = Window;
	Parameters.Windowed = !Fullscreen;
	Parameters.EnableAutoDepthStencil = true;
	Parameters.AutoDepthStencilFormat = D3DFMT_D24S8;
	Parameters.Flags = NULL;

	Parameters.FullScreen_RefreshRateInHz = 0;
	Parameters.PresentationInterval = Fullscreen ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_ONE;
#pragma endregion

#pragma region //Pre-Processor directives and device creation
#ifdef FREE
	Parameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
#endif

#ifdef PERF
	uint32 AdapterToUse = D3DADAPTER_DEFAULT;
	D3DDEVTYPE DeviceType = D3DDEVTYPE_HAL;

	for (UINT Adapter = 0; Adapter < Context->GetAdapterCount(); Adapter++)
	{
		D3DADAPTER_IDENTIFIER9 Identifier;
		HRESULT Res;

		Res = Context->GetAdapterIdentifier(Adapter, 0, &Identifier);

		if (strstr(Identifier.Description,"PerfHUD") != 0)
		{
			AdapterToUse = Adapter;
			DeviceType = D3DDEVTYPE_REF;
			break;
		}
	}

	HRESULT result = Context->CreateDevice(AdapterToUse, DeviceType, Window, D3DCREATE_HARDWARE_VERTEXPROCESSING, &Parameters, &Device);
#else
	HRESULT result = Context->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, Window, D3DCREATE_HARDWARE_VERTEXPROCESSING, &Parameters, &Device);
#endif
#pragma endregion

	//Check Device creation for error
	errorCheck(result, L"DeviceManager Constructor: Failed to create device!\r\n");
	
	//Initialize resources
	onDeviceReset();
}


DeviceManager::~DeviceManager(void)
{
	SAFE_RELEASE(Query);
	SAFE_RELEASE(MeshVD);
	SAFE_RELEASE(DepthBuffer);
	SAFE_RELEASE(BackBuffer);
	SAFE_RELEASE(Device);
	SAFE_RELEASE(Context);
}

void DeviceManager::onDeviceLost()
{
	if(OnDeviceLost) OnDeviceLost();

	SAFE_RELEASE(MeshVD);
	SAFE_RELEASE(DepthBuffer);
	SAFE_RELEASE(BackBuffer);
	SAFE_RELEASE(Query);
}

void DeviceManager::onDeviceReset()
{
	Device->CreateQuery(D3DQUERYTYPE_EVENT, &Query);
	Device->GetRenderTarget(0, &BackBuffer);
	Device->GetDepthStencilSurface(&DepthBuffer);

	{
		D3DVERTEXELEMENT9 ve[] =
		{
			{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
			{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
			{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
			{0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0},
			{0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0},
			D3DDECL_END()
		};

		Device->CreateVertexDeclaration(ve, &MeshVD);
	}

	if(OnDeviceReset) OnDeviceReset();
}

void DeviceManager::changeViewMode( int Width, int Height, bool Fullscreen )
{
	Parameters.BackBufferWidth = Width;
	Parameters.BackBufferHeight = Height;
	Parameters.Windowed = !Fullscreen;

	if(Fullscreen) 
		Parameters.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	else
		Parameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	Parameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	onDeviceLost();

	if(Device->Reset(&Parameters) != D3D_OK) 
		return;

	onDeviceReset();
}

void DeviceManager::errorCheck( HRESULT result, LPCTSTR debugInfo )
{
	if(result == D3D_OK) return;

	LPTSTR text;

#pragma region Switch check
	switch(result)
	{
	case D3DERR_WRONGTEXTUREFORMAT:
		text = TEXT("D3DERR_WRONGTEXTUREFORMAT");
		MessageBox(NULL, TEXT("D3DERR_WRONGTEXTUREFORMAT"), TEXT("DirectX error!"), MB_OK | MB_ICONERROR);
		break;
	case D3DERR_UNSUPPORTEDCOLOROPERATION:
		text = TEXT("D3DERR_UNSUPPORTEDCOLOROPERATION");
		MessageBox(NULL, TEXT("D3DERR_UNSUPPORTEDCOLOROPERATION"), TEXT("DirectX error!"), MB_OK | MB_ICONERROR);
		break;
	case D3DERR_UNSUPPORTEDCOLORARG:
		text = TEXT("D3DERR_UNSUPPORTEDCOLORARG");
		MessageBox(NULL, TEXT("D3DERR_UNSUPPORTEDCOLORARG"), TEXT("DirectX error!"), MB_OK | MB_ICONERROR);
		break;
	case D3DERR_UNSUPPORTEDALPHAOPERATION:
		text = TEXT("D3DERR_UNSUPPORTEDALPHAOPERATION");
		MessageBox(NULL, TEXT("D3DERR_UNSUPPORTEDALPHAOPERATION"), TEXT("DirectX error!"), MB_OK | MB_ICONERROR);
		break;
	case D3DERR_UNSUPPORTEDALPHAARG:
		text = TEXT("D3DERR_UNSUPPORTEDALPHAARG");
		MessageBox(NULL, TEXT("D3DERR_UNSUPPORTEDALPHAARG"), TEXT("DirectX error!"), MB_OK | MB_ICONERROR);
		break;
	case D3DERR_TOOMANYOPERATIONS:
		text = TEXT("D3DERR_TOOMANYOPERATIONS");
		MessageBox(NULL, TEXT("D3DERR_TOOMANYOPERATIONS"), TEXT("DirectX error!"), MB_OK | MB_ICONERROR);
		break;
	case D3DERR_CONFLICTINGTEXTUREFILTER:
		text = TEXT("D3DERR_CONFLICTINGTEXTUREFILTER");
		MessageBox(NULL, TEXT("D3DERR_CONFLICTINGTEXTUREFILTER"), TEXT("DirectX error!"), MB_OK | MB_ICONERROR);
		break;
	case D3DERR_UNSUPPORTEDFACTORVALUE:
		text = TEXT("D3DERR_UNSUPPORTEDFACTORVALUE");
		MessageBox(NULL, TEXT("D3DERR_UNSUPPORTEDFACTORVALUE"), TEXT("DirectX error!"), MB_OK | MB_ICONERROR);
		break;
	case D3DERR_CONFLICTINGRENDERSTATE:
		text = TEXT("D3DERR_CONFLICTINGRENDERSTATE");
		MessageBox(NULL, TEXT("D3DERR_CONFLICTINGRENDERSTATE"), TEXT("DirectX error!"), MB_OK | MB_ICONERROR);
		break;
	case D3DERR_UNSUPPORTEDTEXTUREFILTER:
		text = TEXT("D3DERR_UNSUPPORTEDTEXTUREFILTER");
		MessageBox(NULL, TEXT("D3DERR_UNSUPPORTEDTEXTUREFILTER"), TEXT("DirectX error!"), MB_OK | MB_ICONERROR);
		break;
	case D3DERR_CONFLICTINGTEXTUREPALETTE:
		text = TEXT("D3DERR_CONFLICTINGTEXTUREPALETTE");
		MessageBox(NULL, TEXT("D3DERR_CONFLICTINGTEXTUREPALETTE"), TEXT("DirectX error!"), MB_OK | MB_ICONERROR);
		break;
	case D3DERR_DRIVERINTERNALERROR:
		text = TEXT("D3DERR_DRIVERINTERNALERROR");
		MessageBox(NULL, TEXT("D3DERR_DRIVERINTERNALERROR"), TEXT("DirectX error!"), MB_OK | MB_ICONERROR);
		break;

	case D3DERR_NOTFOUND:
		text = TEXT("D3DERR_NOTFOUND");
		MessageBox(NULL, TEXT("D3DERR_NOTFOUND"), TEXT("DirectX error!"), MB_OK | MB_ICONERROR);
		break;
	case D3DERR_MOREDATA:
		text = TEXT("D3DERR_MOREDATA");
		MessageBox(NULL, TEXT("D3DERR_MOREDATA"), TEXT("DirectX error!"), MB_OK | MB_ICONERROR);
		break;
	case D3DERR_DEVICELOST:
		text = TEXT("D3DERR_DEVICELOST");
		MessageBox(NULL, TEXT("D3DERR_DEVICELOST"), TEXT("DirectX error!"), MB_OK | MB_ICONERROR);
		break;
	case D3DERR_DEVICENOTRESET:
		text = TEXT("D3DERR_DEVICENOTRESET");
		MessageBox(NULL, TEXT("D3DERR_DEVICENOTRESET"), TEXT("DirectX error!"), MB_OK | MB_ICONERROR);
		break;
	case D3DERR_NOTAVAILABLE:
		text = TEXT("D3DERR_NOTAVAILABLE");
		MessageBox(NULL, TEXT("D3DERR_NOTAVAILABLE"), TEXT("DirectX error!"), MB_OK | MB_ICONERROR);
		break;
	case D3DERR_OUTOFVIDEOMEMORY:
		text = TEXT("D3DERR_OUTOFVIDEOMEMORY");
		MessageBox(NULL, TEXT("D3DERR_OUTOFVIDEOMEMORY"), TEXT("DirectX error!"), MB_OK | MB_ICONERROR);
		break;
	case D3DERR_INVALIDCALL:
		text = TEXT("D3DERR_INVALIDCALL");
		MessageBox(NULL, TEXT("D3DERR_INVALIDCALL"), TEXT("DirectX error!"), MB_OK | MB_ICONERROR);
		break;
	case D3DERR_DRIVERINVALIDCALL:
		text = TEXT("D3DERR_DRIVERINVALIDCALL");
		MessageBox(NULL, TEXT("D3DERR_DRIVERINVALIDCALL"), TEXT("DirectX error!"), MB_OK | MB_ICONERROR);
		break;
	case D3DERR_WASSTILLDRAWING:
		text = TEXT("D3DERR_WASSTILLDRAWING");
		MessageBox(NULL, TEXT("D3DERR_WASSTILLDRAWING"), TEXT("DirectX error!"), MB_OK | MB_ICONERROR);
		break;

	case D3DERR_DEVICEREMOVED:
		text = TEXT("D3DERR_DEVICEREMOVED");
		MessageBox(NULL, TEXT("D3DERR_DEVICEREMOVED"), TEXT("DirectX error!"), MB_OK | MB_ICONERROR);
		break;		
	case D3DERR_DEVICEHUNG:
		text = TEXT("D3DERR_DEVICEHUNG");
		MessageBox(NULL, TEXT("D3DERR_DEVICEHUNG"), TEXT("DirectX error!"), MB_OK | MB_ICONERROR);
		break;

	default:
		text = TEXT("D3DERR_UNSPECIFIED");
		MessageBox(NULL, TEXT("D3DERR_UNSPECIFIED"), TEXT("DirectX error!"), MB_OK | MB_ICONERROR);
		break;
	}
#pragma endregion

	LPTSTR error = new TCHAR[256];
	wsprintf(error, TEXT("%s\r\n%s"), debugInfo, text);
	throw(error);
}

void DeviceManager::Present()
{
	Device->Present(0, 0, 0, 0);

	Query->Issue(D3DISSUE_END);
	Query->GetData( NULL, 0, D3DGETDATA_FLUSH );

	while(Query->GetData(NULL, 0, D3DGETDATA_FLUSH) == S_FALSE)
	{
		if(Query->GetData(NULL, 0, D3DGETDATA_FLUSH) == D3DERR_DEVICELOST) break;
	}
}