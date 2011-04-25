#ifndef _GDS_H_RENDERERDX9_
#define _GDS_H_RENDERERDX9_

#include "GdsRendererBase.h"
#include "../Camera/GdsCameraManagerDX9.h"
#include "../Property/GdsProperty.h"
#include "GdsRenderFrame.h"

class GdsRendererDX9 : public GdsRendererBase
{

private:
	
 	LPDIRECT3D9             m_pD3D;      // = NULL; // Used to create the D3DDevice
 	LPDIRECT3DDEVICE9       m_pd3dDevice;// = NULL; // Our rendering device
	GdsNodePtr				m_RootNode;
	bool					m_bIsWireFrame;

	std::vector< GdsPropertyPtr>	m_RenderList;

	GdsRenderFramePtr		m_RenderFrame;

	void					setRootNodeAndCamNode();
protected:

	virtual	void			vClear();
	virtual HRESULT			vCreate( HWND hWnd );
	virtual HRESULT			vRelease();

public:

	GdsRendererDX9();
	virtual ~GdsRendererDX9();
	
	void					Clear(){ vClear(); }
	HRESULT					Create( HWND hWnd ){ return vCreate( hWnd ); }
	HRESULT					Release(){ return vRelease(); }

	void					SetRootNode( GdsNodePtr node ){ m_RootNode = node; }
	GdsNodePtr				GetRootNode() { return m_RootNode; }
	
	void					vUpdate( float fAccumTime );
	LPDIRECT3DDEVICE9		GetDevice()		{ return m_pd3dDevice; }

	void					SetWireFrame( bool flag ){ m_bIsWireFrame = flag; }
	void					AddRenderToken( GdsPropertyPtr rendertoken );

	GdsRenderFramePtr		GetRenderFrame(){ return m_RenderFrame; }

};

typedef boost::shared_ptr< GdsRendererDX9 > GdsRendererDX9Ptr;

#define RENDERER	singleton_holder< GdsRendererDX9 >::instance()

#endif