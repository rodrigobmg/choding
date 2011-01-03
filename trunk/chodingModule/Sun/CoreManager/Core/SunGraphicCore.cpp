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

	//DX9���� ����, ���� ���ÿ� ���� DX10, DX11���������ϵ���
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

