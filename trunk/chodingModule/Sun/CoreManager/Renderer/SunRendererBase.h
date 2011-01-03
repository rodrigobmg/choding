#pragma once


typedef enum _eRenderID
{
	RENDERER_DX9  = 0,
	RENDERER_DX10,
	RENDERER_DX11,
} eRenderID;


//랜더러 베이스 클레스
class DLL_API Sun_CRendererBase
{
public:
	Sun_CRendererBase();
	virtual ~Sun_CRendererBase();

public:
	virtual HRESULT InitializeRenderer() = 0;
	virtual void    DestoryRenderer()	 = 0;

	virtual bool	BeginRender()	= 0;
	virtual bool    EndRender()		= 0;


protected:
	eRenderID		m_eRendererID;

};