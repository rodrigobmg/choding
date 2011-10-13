#ifndef _SNOWBOARD_RENDERER_MANAGER_H_
#define _SNOWBOARD_RENDERER_MANAGER_H_

#include "GdsRendererDX9.h"
#include "GdsRenderFrame.h"

class GdsRendererManager
{	
public:

	GdsRendererManager();
	virtual ~GdsRendererManager();

	bool			EnableRendering();
	void			SetRenderingEnable( bool flag );
	bool			Render( float fAccumtime );
	
	bool			Create( HWND hwnd );

	GdsRendererDX9*	GetRenderer9(){ return m_pRenderer; }

	void			DrawBox( D3DXMATRIX& mat , D3DXVECTOR3& minPos , D3DXVECTOR3& maxPos );
	void			DrawAxis( D3DXMATRIX& mat , D3DXVECTOR3& point );

	unsigned long	GetProcessTick(){ return m_ProcessTick; }
	int				GetCountOfRenderObject(){ return m_CountRenderObject; }

private:
	
	bool			m_bUseThreadRender;
	unsigned long	m_ProcessTick;
	GdsRendererDX9*	m_pRenderer;
	bool			m_IsCurRendering;
	int				m_CountRenderObject;

};

#define RENDERER	singleton_holder< GdsRendererManager >::instance()

#endif