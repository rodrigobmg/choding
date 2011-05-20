#ifndef _GDS_H_RENDERSTATEGROUP_H_
#define _GDS_H_RENDERSTATEGROUP_H_

#include "../Base/GdsObject.h"

#define TERRAIN_RENDERSTATE	0

class GdsRenderStateGroup : public GdsObject
{	
	typedef std::pair< int , int >				VALUE;
	typedef std::vector< VALUE >				OPT_RENDERSTATE;

	typedef std::pair< int , VALUE >			MULTI_VALUE;
	typedef std::vector< MULTI_VALUE >			OPT_MULTITEXTURE;
	
	OPT_RENDERSTATE								m_Opt_RenderState;	
	OPT_MULTITEXTURE							m_Opt_MultiTexturing;
	OPT_MULTITEXTURE							m_Opt_SamplerState;

public:

	GdsRenderStateGroup(){ SetName( OBJECT_RENDERSTATEGROUP ); }
	virtual ~GdsRenderStateGroup();

	void			SetRenderState( int renderstste_key , int renderstate_value );	
	void			ClearRenderStateOpt(){ m_Opt_RenderState.clear(); }

	void			SetTextureStageState( int index , int iop1 , int iop2 );
	void			ClearTextureStageStateOpt(){ m_Opt_MultiTexturing.clear(); }

	void			SetSamplerState( int index , int iop1 , int iop2 );
	void			ClearSamplerStateOpt(){ m_Opt_SamplerState.clear(); }

	void			SetRenderState( LPDIRECT3DDEVICE9 device );

};

typedef boost::shared_ptr< GdsRenderStateGroup > GdsRenderStateGroupPtr;

#endif