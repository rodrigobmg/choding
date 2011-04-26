#ifndef _GDS_H_RENDERFRAME_H_
#define _GDS_H_RENDERFRAME_H_

#include "../Base/GdsObject.h"
#include "GdsRenderObject.h"
#include "boost/tuple/tuple.hpp"

class GdsRenderFrame : public GdsObject
{	
	typedef std::pair< int , int >				VALUE;
	typedef std::vector< VALUE >				OPT_RENDERSTATE;
	//typedef tuple< int , int , int >			VALUE_MULTITEXTURE;

	typedef std::pair< int , VALUE >			MULTI_VALUE;
	typedef std::vector< MULTI_VALUE >			OPT_MULTITEXTURE;
	
	typedef std::list< GdsRenderObjectPtr >		RENDEROBJECT;

	OPT_RENDERSTATE								m_Opt_RenderState;	
	RENDEROBJECT								m_RenderFrame;

	OPT_MULTITEXTURE							m_Opt_MultiTexturing;
	OPT_MULTITEXTURE							m_Opt_SamplerState;

public:

	GdsRenderFrame(){ SetName( OBJECT_RENDERFRAME ); }
	virtual ~GdsRenderFrame();

	GdsRenderObjectPtr	AllocRenderObject();
	void				FreeRenderObejct();

	void			AttachRenderObject( GdsRenderObjectPtr pRenderObject );
	void			DetachRenderObject( GdsRenderObjectPtr pRenderObject );

	void			SetRenderState( int renderstste_key , int renderstate_value );	
	void			ClearRenderStateOpt(){ m_Opt_RenderState.clear(); }

	void			SetTextureStageState( int index , int iop1 , int iop2 );
	void			ClearTextureStageStateOpt(){ m_Opt_MultiTexturing.clear(); }

	void			SetSamplerState( int index , int iop1 , int iop2 );
	void			ClearSamplerState(){ m_Opt_SamplerState.clear(); }

	void			Render( LPDIRECT3DDEVICE9 device ){ vRender( device ); }

protected:

	virtual	void	vRender( LPDIRECT3DDEVICE9 device );

};

typedef boost::shared_ptr< GdsRenderFrame > GdsRenderFramePtr;

#endif