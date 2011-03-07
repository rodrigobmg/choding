#ifndef _GDS_H_RENDERERDX9_
#define _GDS_H_RENDERERDX9_

#include "GdsRendererBase.h"

class GdsNode;

class GdsRendererDX9 : public GdsRendererBase{

private:
	
 	LPDIRECT3D9             m_pD3D;      // = NULL; // Used to create the D3DDevice
 	LPDIRECT3DDEVICE9       m_pd3dDevice;// = NULL; // Our rendering device


public:

	GdsRendererDX9();
	virtual ~GdsRendererDX9();
	
	virtual	void			Clear();
	virtual HRESULT			Create( HWND hWnd );
	virtual HRESULT			Release();

	virtual void			Render( GdsNodePtr pNode );
	LPDIRECT3DDEVICE9		GetDevice()		{ return m_pd3dDevice; }

};

typedef boost::shared_ptr< GdsRendererDX9 > GdsRendererDX9Ptr;

#endif