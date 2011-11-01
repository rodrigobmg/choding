#include "StdAfx.h"
#include "RenderSystem.h"
#include "RenderObject\IRenderObject.h"
#include "RenderObject\RenderObject.h"

RenderSystem::RenderSystem(void)
:m_pDeviceManager(NULL),
m_pCameraManager(NULL)
{
}


RenderSystem::~RenderSystem(void)
{
}

IRenderObject* RenderSystem::AllocRenderObject( IRenderObject::ENABLE_RENDER_TYPE type )
{
	IRenderObject* pRenderObject = NULL;
	switch( type )
	{
	case IRenderObject::MESH:
		pRenderObject = new RenderObject;
	}

	if( pRenderObject != NULL )
	{
		m_RenderObjectContainer.push_back( pRenderObject );
	}

	return pRenderObject;
}

bool RenderSystem::ReleaseRenderObject()
{
	return true;
}

void RenderSystem::Render()
{	
	if( m_pDeviceManager == NULL )
		return;

	if( m_pCameraManager )
	{
		m_pCameraManager->Update( 0.f );
		m_pDeviceManager->getDevice()->SetTransform( D3DTS_VIEW , &(m_pCameraManager->GetCurCam().View) );
		m_pDeviceManager->getDevice()->SetTransform( D3DTS_PROJECTION, &(m_pCameraManager->GetCurCam().Projection) );
	}	
	
	D3DDevice* pDevice = m_pDeviceManager->getDevice();

	typedef	std::vector< IRenderObject* >::iterator renderObjectIter;
	renderObjectIter itB = m_RenderObjectContainer.begin();
	renderObjectIter itE = m_RenderObjectContainer.end();
	for (renderObjectIter it = itB ; it != itE ; ++it )
	{
		(*it)->Draw( pDevice , NULL );
	}

	m_RenderObjectContainer.clear();
}

DeviceManager* RenderSystem::GetDeviceManager()
{
	return m_pDeviceManager;
}

void RenderSystem::onDeviceLost()
{

}

void RenderSystem::onDeviceReset()
{

}

DeviceManager* RenderSystem::CreateDeviceManager( HWND Window, UINT Width, UINT Height, bool Fullscreen )
{
	m_pDeviceManager = new DeviceManager( Window , Width ,Height , Fullscreen );
	return m_pDeviceManager;
}

void RenderSystem::SetCamera( Camera* camera )
{
	if ( m_pCameraManager == NULL )
		return;

	m_pCameraManager->SetCurCam( camera );
}
