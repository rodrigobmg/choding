#ifndef _RENDERER_BASE_H_
#define _RENDERER_BASE_H_

class RendererBase//: public GdsMemObject
{	

public:	
	virtual ~RendererBase(){};

	void				Render( float fAcuumTime );
	virtual	void		Clear() = 0;
	virtual bool		Create( HWND hWnd ) = 0;
	virtual HRESULT		Release() = 0;

protected:	
	RendererBase();
	virtual void		RenderFrame( float fAccumtime ) = 0;
};

#endif