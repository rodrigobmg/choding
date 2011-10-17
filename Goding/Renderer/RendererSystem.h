#ifndef _SNOWBOARD_RENDERER_MANAGER_H_
#define _SNOWBOARD_RENDERER_MANAGER_H_

#include "RendererDX9.h"
#include "RenderFrame.h"

class RendererSystem
{	
public:

	RendererSystem();
	virtual ~RendererSystem();

	bool			EnableRendering();
	void			SetRenderingEnable( bool flag );
	bool			Render( float fAccumtime );
	
	bool			Create( HWND hwnd );

	RendererDX9*	GetRenderer9(){ return m_pRenderer; }

	void			DrawBox( D3DXMATRIX& mat , D3DXVECTOR3& minPos , D3DXVECTOR3& maxPos );
	void			DrawAxis( D3DXMATRIX& mat , D3DXVECTOR3& point );

	unsigned long	GetProcessTick(){ return m_ProcessTick; }
	int				GetCountOfRenderObject(){ return m_CountRenderObject; }

private:
	
	bool			m_bUseThreadRender;
	unsigned long	m_ProcessTick;
	RendererDX9*	m_pRenderer;
	bool			m_IsCurRendering;
	int				m_CountRenderObject;

};

#define RENDERER	singleton_holder< RendererSystem >::instance()

#endif