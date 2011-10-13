#ifndef _SNOWBOARD_RENDERER_BASE_H_
#define _SNOWBOARD_RENDERER_BASE_H_

class GdsRendererBase//: public GdsMemObject
{	

public:	
	virtual ~GdsRendererBase(){};

	void				Render( float fAcuumTime );
	virtual	void		Clear() = 0;
	virtual bool		Create( HWND hWnd ) = 0;
	virtual HRESULT		Release() = 0;

protected:	
	GdsRendererBase();
	virtual void		RenderFrame( float fAccumtime ) = 0;
};

#endif