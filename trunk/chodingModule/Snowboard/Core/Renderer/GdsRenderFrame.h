#ifndef _GDS_H_RENDERFRAME_H_
#define _GDS_H_RENDERFRAME_H_

#include "../Base/GdsObject.h"
#include "GdsRenderObject.h"
#include "GdsRenderStateGroup.h"

class GdsRenderFrame : public GdsObject
{	
	typedef std::pair< int , GdsRenderObject* >	RENDEROBEJCT;

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

	
	typedef std::vector< GdsRenderObject* >		RENDERTOKEN_LIST;
	RENDERTOKEN_LIST		m_RenderToken;

	typedef std::list< RENDEROBEJCT	>					RENDER_CONTAINER;	
	RENDER_CONTAINER		m_RenderFrame;
	RENDER_CONTAINER		m_FrontRenderFrame;

	typedef std::map< int , GdsRenderStateGroupPtr >	RENDERSTATEGROUP;
	RENDERSTATEGROUP		m_RenderStateList;

	int						m_iTotalcountofRenderObject;

	//그림을 그릴 전면 렌더프레임으로 스왑
	void					swap_buffer();

public:

	GdsRenderFrame();
	virtual ~GdsRenderFrame();

	int						GetRenderObjectCount(){ return m_iTotalcountofRenderObject; }
	GdsRenderObject*		AllocRenderObject();
	void					FreeRenderObject( GdsRenderObject* p );
	void					AttachRenderObject( GdsRenderObject* pRenderObject , int iRenderStateGroupID );
	void					AddRenderStateGroup( GdsRenderStateGroupPtr renderstategroup , int iRenderStateGroupID );
	
	void					Render( LPDIRECT3DDEVICE9 device ){ vRender( device ); }
	

	

protected:

	virtual	void			vRender( LPDIRECT3DDEVICE9 device );

};

typedef boost::shared_ptr< GdsRenderFrame > GdsRenderFramePtr;

#endif