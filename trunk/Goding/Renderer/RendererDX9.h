#ifndef _RENDERER_DX9_H_
#define _RENDERER_DX9_H_

#include "RendererBase.h"
#include "RenderFrame.h"

class RendererDX9 : public RendererBase
{
	//상자나 축그릴때 사용할 녀석들
	typedef boost::tuple< D3DXMATRIX , D3DXVECTOR3 >				AXISDATA;
	typedef std::list< AXISDATA >									LIST_AXIS;
	LIST_AXIS					m_listAxisData;
	typedef	boost::tuple< D3DXMATRIX , D3DXVECTOR3 , D3DXVECTOR3 >	BOXDATA;
	typedef std::list< BOXDATA >									LIST_BOX;
	LIST_BOX					m_listBoxData;

private:
	
 	LPDIRECT3D9             m_pD3D;      // = NULL; // Used to create the D3DDevice
 	LPDIRECT3DDEVICE9       m_pd3dDevice;// = NULL; // Our rendering device
	bool					m_bWireMode;

	RenderFramePtr 			m_RenderFrameList;

	void					drawEtc();
	void					drawAxis( D3DXMATRIX& mat , D3DXVECTOR3& point , ID3DXLine* line );
	void					drawBox( D3DXMATRIX& mat , D3DXVECTOR3& minPos , D3DXVECTOR3& maxPos , ID3DXLine* line );

public:

	RendererDX9();
	virtual ~RendererDX9();

	virtual void			Clear();
	virtual bool			Create( HWND hWnd );
	virtual HRESULT			Release();

	void					DrawBox( D3DXMATRIX& mat , D3DXVECTOR3& minPos , D3DXVECTOR3& maxPos );
	void					DrawAxis( D3DXMATRIX& mat , D3DXVECTOR3& point );
	
	void					ToggleWireMode(){ m_bWireMode = !m_bWireMode; }
	
	void					RenderFrame( float fAccumtime );
	LPDIRECT3DDEVICE9		GetDevice()		{ return m_pd3dDevice; }

	RenderFramePtr		GetRenderFrame(){ return m_RenderFrameList; }
};

typedef boost::shared_ptr< RendererDX9 > RendererDX9Ptr;

#endif