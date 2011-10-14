#ifndef _SNOWBOARD_RENDERER_RENDERFRAME_H_
#define _SNOWBOARD_RENDERER_RENDERFRAME_H_

#include "GdsRenderObject.h"
#include "GdsRenderStateGroup.h"

class GdsRenderFrame// : public GdsMemObject
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

	typedef std::list< RENDEROBEJCT	>					RENDER_CONTAINER;	
	RENDER_CONTAINER		m_RenderFrame;
	RENDER_CONTAINER		m_FrontRenderFrame;

	typedef std::map< int , GdsRenderStateGroupPtr >	RENDERSTATEGROUP;
	RENDERSTATEGROUP		m_RenderStateList;

	int						m_iTotalcountofRenderObject;

public:

	GdsRenderFrame();
	virtual ~GdsRenderFrame();
	
	//�׸��� �׸� ���� �������������� ����
	void					Swap_buffer();

	int						GetRenderObjectCount(){ return m_iTotalcountofRenderObject; }
	GdsRenderObject*		AllocRenderObject( int iRenderStateGroupID );
	void					ClearBackFrameBuffer();
	void					AttachRenderObject( GdsRenderObject* pRenderObject , int iRenderStateGroupID );
	void					AddRenderStateGroup( GdsRenderStateGroupPtr renderstategroup , int iRenderStateGroupID );
	
	void					Render( LPDIRECT3DDEVICE9 device ){ vRender( device ); }
	
protected:

	virtual	void			vRender( LPDIRECT3DDEVICE9 device );

};

typedef boost::shared_ptr< GdsRenderFrame > GdsRenderFramePtr;

#endif