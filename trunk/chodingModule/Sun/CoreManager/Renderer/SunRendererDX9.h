#pragma once


class DLL_API Sun_CRendererDX9 : public Sun_CRendererBase
{
public:
	Sun_CRendererDX9();
	virtual ~Sun_CRendererDX9();

public:

	virtual HRESULT InitializeRenderer();
	virtual void    DestoryRenderer();

	virtual bool	BeginRender();
	virtual bool    EndRender()	;

protected:
	LPDIRECT3DDEVICE9	m_pD3DDevice;
	LPDIRECT3D9			m_pD3D;



};