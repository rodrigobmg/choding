#ifndef _GDS_CORE_RENDERER_RENDERERMANAGER_H_
#define _GDS_CORE_RENDERER_RENDERERMANAGER_H_

#include "../Base/GdsObject.h"
#include "GdsRendererDX9.h"
#include "GdsRenderFrame.h"

class GdsRendererManager : public GdsObject{	

public:

	GdsRendererManager();
	virtual ~GdsRendererManager();

	bool			EnableRendering();
	void			SetRenderingEnable( bool flag );
	void			Render( float fAcuumTime );
	
	bool			Create( HWND hwnd );

	GdsRendererDX9*	GetRenderer9(){ return m_pRenderer; }

	void			DrawBox( D3DXMATRIX& mat , D3DXVECTOR3& minPos , D3DXVECTOR3& maxPos ){ m_pRenderer->DrawBox( mat , minPos , maxPos ); }
	void			DrawAxis( D3DXMATRIX& mat , D3DXVECTOR3& point ){ m_pRenderer->DrawAxis( mat , point ); }

private:

	GdsRendererDX9*	m_pRenderer;
	bool			m_IsCurRendering;
};

#define RENDERER	singleton_holder< GdsRendererManager >::instance()

#endif