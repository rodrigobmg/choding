#ifndef _GDS_H_RENDERFRAME_H_
#define _GDS_H_RENDERFRAME_H_

#include "../Base/GdsObject.h"
#include "GdsRenderObject.h"
#include "GdsRenderStateGroup.h"

class GdsRenderFrame : public GdsObject
{	
	typedef std::pair< int , GdsRenderObjectPtr >	RENDEROBEJCT;
	typedef std::vector< RENDEROBEJCT >				RENDER_CONTAINER;
	
	RENDER_CONTAINER		m_RenderFrame;

	typedef std::map< int , GdsRenderStateGroupPtr >	RENDERSTATEGROUP;
	RENDERSTATEGROUP		m_RenderStateList;

public:

	GdsRenderFrame(){ SetName( OBJECT_RENDERFRAME ); }
	virtual ~GdsRenderFrame();

	GdsRenderObjectPtr		AllocRenderObject();
	void					FreeRenderObejct();

	void					AttachRenderObject( GdsRenderObjectPtr pRenderObject , int iRenderStateGroupID );
	void					DetachRenderObject( GdsRenderObjectPtr pRenderObject );

	void					AddRenderStateGroup( GdsRenderStateGroupPtr renderstategroup , int iRenderStateGroupID );

	void					Render( LPDIRECT3DDEVICE9 device ){ vRender( device ); }

protected:

	virtual	void			vRender( LPDIRECT3DDEVICE9 device );

};

typedef boost::shared_ptr< GdsRenderFrame > GdsRenderFramePtr;

#endif