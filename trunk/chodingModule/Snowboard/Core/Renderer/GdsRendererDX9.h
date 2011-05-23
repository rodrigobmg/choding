#ifndef _GDS_H_RENDERERDX9_
#define _GDS_H_RENDERERDX9_

#include "GdsRendererBase.h"
#include "../Camera/GdsCameraManagerDX9.h"
#include "GdsRenderFrame.h"

class GdsRendererDX9 : public GdsRendererBase
{

	typedef std::pair< D3DXVECTOR3 , D3DXVECTOR3 >	RECT_DATA;
	typedef std::list< RECT_DATA >					DRAWRECT_CONTAINER;
	DRAWRECT_CONTAINER		m_DrawRectData;

	typedef std::list< D3DXVECTOR3 >				DRAWPOINT_CONTAINER;
	DRAWPOINT_CONTAINER		m_DrawPointData;

private:
	
 	LPDIRECT3D9             m_pD3D;      // = NULL; // Used to create the D3DDevice
 	LPDIRECT3DDEVICE9       m_pd3dDevice;// = NULL; // Our rendering device
	GdsNodePtr				m_RootNode;
	bool					m_bWireMode;

	GdsRenderFramePtr 		m_RenderFrameList;

	void					setRootNodeAndCamNode();


	void					drawAxis( D3DXVECTOR3& point );
	void					drawBox( D3DXVECTOR3& minPos , D3DXVECTOR3& maxPos );

protected:

	virtual	void			vClear();
	virtual bool			vCreate( HWND hWnd );
	virtual HRESULT			vRelease();

public:

	GdsRendererDX9();
	virtual ~GdsRendererDX9();

	void					DrawRect( D3DXVECTOR3& minPos , D3DXVECTOR3& maxPos );
	void					DrawPoint( D3DXVECTOR3& point );
	
	void					ToggleWireMode(){ m_bWireMode = !m_bWireMode; }
	void					Clear(){ vClear(); }
	bool					Create( HWND hWnd ){ return vCreate( hWnd ); }
	HRESULT					Release(){ return vRelease(); }

	void					SetRootNode( GdsNodePtr node ){ m_RootNode = node; }
	GdsNodePtr				GetRootNode() { return m_RootNode; }
	
	void					vUpdate( float fAccumTime );
	LPDIRECT3DDEVICE9		GetDevice()		{ return m_pd3dDevice; }

	GdsRenderFramePtr		GetRenderFrame(){ return m_RenderFrameList; }
};

typedef boost::shared_ptr< GdsRendererDX9 > GdsRendererDX9Ptr;

#define RENDERER	singleton_holder< GdsRendererDX9 >::instance()

#endif