#include "Precompiled.h"
#include "sunRenderer.h"


sunRenderer::sunRenderer():
m_iPolygonCount(0),
m_hWnd(NULL),
m_bIsWireFrame(false)
{
}


sunRenderer::~sunRenderer()
{

}


eRenderID sunRenderer::GetRendererID()
{
	return m_eRendererID;
}



