#ifndef _SUN_COREMANAGER_RENDERER_RENDERER_H_
#define _SUN_COREMANAGER_RENDERER_RENDERER_H_



typedef enum _eRenderID
{	
	RENDERER_DX9 = 0,
	RENDERER_DX10 ,
	RENDERER_DX11 ,

} eRenderID;


class DLL_API sunRenderer
{

public:
	sunRenderer();
	~sunRenderer();


public:
	virtual HRESULT InitializeRenderer() = 0;
	virtual void	DestroyRenderer() = 0;
	
	virtual void RenderObject( const vector<sunRenderNodePtr>& RenderList ) = 0;
	virtual bool BeginRender() = 0;
	virtual bool EndRender()   = 0;

	virtual void SetCameraData( sunCameraPtr spCamera ) = 0;

public:
	eRenderID GetRendererID();


protected:
	eRenderID		m_eRendererID;
	int				m_iPolygonCount;			//������ ����.
	bool			m_bIsWireFrame;				// ���̾� ������ ��� ����

protected:
	HWND				m_hWnd;

};


#endif