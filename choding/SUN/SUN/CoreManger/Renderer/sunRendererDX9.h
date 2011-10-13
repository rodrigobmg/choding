#ifndef _SUN_COREMANAGER_RENDERER_RENDERERDX9_H_
#define _SUN_COREMANAGER_RENDERER_RENDERERDX9_H_


class DLL_API sunRendererDX9 : public sunRenderer
{	

public:
	sunRendererDX9();
	~sunRendererDX9();

public:
	virtual HRESULT InitializeRenderer();
	virtual void	DestroyRenderer();

	
	virtual bool	BeginRender();
	virtual void	RenderObject( const vector<sunRenderNodePtr>& RenderList );
	virtual bool	EndRender();

	virtual void    SetCameraData( sunCameraPtr spCamera );

public:
	inline LPDIRECT3DDEVICE9 GetDevice() {return m_pD3DDevice;}

public:
	HRESULT SetDefaultState();


protected:
	LPDIRECT3DDEVICE9	m_pD3DDevice;

	LPDIRECT3D9			m_pD3D;
	LPD3DXFONT			m_pFont;

	IDirect3DStateBlock9*	m_pD3DDefaultSatate;

	D3DCAPS9			m_pD3DCaps;
	D3DSURFACE_DESC		m_D3DBackDesc;

	sunCameraPtr		m_spProgressCamera;

private:
	

};


#endif