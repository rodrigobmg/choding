#ifndef _RENDERER_RENDERSTATEGROUP_H_
#define _RENDERER_RENDERSTATEGROUP_H_

class RenderStateGroup// : public GdsMemObject
{	
	typedef std::pair< int , int >				VALUE;
	typedef std::vector< VALUE >				OPT_RENDERSTATE;

	typedef std::pair< int , VALUE >			MULTI_VALUE;
	typedef std::vector< MULTI_VALUE >			OPT_MULTITEXTURE;
	
	OPT_RENDERSTATE								m_Opt_RenderState;	
	OPT_MULTITEXTURE							m_Opt_MultiTexturing;
	OPT_MULTITEXTURE							m_Opt_SamplerState;

public:

	RenderStateGroup(){}
	virtual ~RenderStateGroup();

	void			SetRenderState( int renderstste_key , int renderstate_value );	
	void			ClearRenderStateOpt(){ m_Opt_RenderState.clear(); }

	void			SetTextureStageState( int index , int iop1 , int iop2 );
	void			ClearTextureStageStateOpt(){ m_Opt_MultiTexturing.clear(); }

	void			SetSamplerState( int index , int iop1 , int iop2 );
	void			ClearSamplerStateOpt(){ m_Opt_SamplerState.clear(); }

	void			SetRenderState( LPDIRECT3DDEVICE9 device );

};

typedef boost::shared_ptr< RenderStateGroup > RenderStateGroupPtr;

#endif