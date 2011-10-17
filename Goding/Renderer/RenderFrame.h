#ifndef _RENDERER_RENDERFRAME_H_
#define _RENDERER_RENDERFRAME_H_

#include "RenderObject.h"
#include "RenderStateGroup.h"

class RenderFrame// : public GdsMemObject
{	
	typedef std::pair< int , RenderObject* >	RENDEROBEJCT;

	struct DataCompare
	{
	public:
		bool operator()( const RENDEROBEJCT& lhs , const RENDEROBEJCT& rhs ) const
		{
			return lhs.first < rhs.first;
		}
		bool operator()( const RENDEROBEJCT& lhs , const int& rhs ) const 
		{
			return lhs.first < rhs;
		}
		bool operator()( const int& lhs , const RENDEROBEJCT& rhs ) const
		{
			return lhs < rhs.first;
		}
	};

	typedef std::list< RENDEROBEJCT	>					RENDER_CONTAINER;	
	RENDER_CONTAINER		m_RenderFrame;
	RENDER_CONTAINER		m_FrontRenderFrame;

	typedef std::map< int , RenderStateGroupPtr >	RENDERSTATEGROUP;
	RENDERSTATEGROUP		m_RenderStateList;

	int						m_iTotalcountofRenderObject;

public:

	RenderFrame();
	virtual ~RenderFrame();
	
	//그림을 그릴 전면 렌더프레임으로 스왑
	void					Swap_buffer();

	int						GetRenderObjectCount(){ return m_iTotalcountofRenderObject; }
	RenderObject*		AllocRenderObject( int iRenderStateGroupID );
	void					ClearBackFrameBuffer();
	void					AttachRenderObject( RenderObject* pRenderObject , int iRenderStateGroupID );
	void					AddRenderStateGroup( RenderStateGroupPtr renderstategroup , int iRenderStateGroupID );
	
	void					Render( LPDIRECT3DDEVICE9 device ){ vRender( device ); }
	
protected:

	virtual	void			vRender( LPDIRECT3DDEVICE9 device );

};

typedef boost::shared_ptr< RenderFrame > RenderFramePtr;

#endif