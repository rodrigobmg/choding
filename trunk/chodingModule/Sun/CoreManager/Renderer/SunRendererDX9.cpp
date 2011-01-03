#include "Precompiled.h"
#include "SunRendererDX9.h"



Sun_CRendererDX9::Sun_CRendererDX9():m_pD3DDevice(NULL), m_pD3D(NULL)
{

}


Sun_CRendererDX9::~Sun_CRendererDX9()
{
	DestoryRenderer();
}

void Sun_CRendererDX9::DestoryRenderer()
{
	Safe_Release( m_pD3DDevice);
	Safe_Release( m_pD3D);
}

HRESULT Sun_CRendererDX9::InitializeRenderer()
{
	//디바이스 생성

	return S_OK;
}

bool Sun_CRendererDX9::BeginRender()
{
	//BeginScene()

	return true;
}

bool Sun_CRendererDX9::EndRender()
{
	//EndScene()
	
	return true;
}