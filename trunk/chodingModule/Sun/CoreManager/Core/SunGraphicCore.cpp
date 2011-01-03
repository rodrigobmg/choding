#include "Precompiled.h"
#include "SunGraphicCore.h"


Sun_CGraphicCore::Sun_CGraphicCore():m_pRenderer(NULL)
{
}

Sun_CGraphicCore::~Sun_CGraphicCore()
{
}


int Sun_CGraphicCore::InitializeCore()
{

	//DX9렌더 생성, 추후 선택에 따라 DX10, DX11지원가능하도록
	m_pRenderer = new Sun_CRendererDX9;

	return 1;
}

void Sun_CGraphicCore::DestoryCore()
{
	Safe_Delete(m_pRenderer);

}

void Sun_CGraphicCore::UpdateCore()
{

}

