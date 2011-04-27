#ifndef _GDS_H_RENDERFRAME_H_
#define _GDS_H_RENDERFRAME_H_

#include "../Base/GdsObject.h"
#include "GdsRenderObject.h"
#include "GdsRenderStateGroup.h"

class GdsRenderFrame : public GdsObject
{	
	typedef std::pair< int , GdsRenderObjectPtr >	RENDEROBEJCT;

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

	
	typedef std::list< RENDEROBEJCT >				RENDER_CONTAINER;
	
	RENDER_CONTAINER		m_RenderFrame;

	typedef std::map< int , GdsRenderStateGroupPtr >	RENDERSTATEGROUP;
	RENDERSTATEGROUP		m_RenderStateList;

	typedef std::list< GdsRenderObjectPtr >			RENDEROBJECT_LIST;
	RENDEROBJECT_LIST		m_DelObjectList;

public:

	GdsRenderFrame(){ SetName( OBJECT_RENDERFRAME ); }
	virtual ~GdsRenderFrame();

	GdsRenderObjectPtr		AllocRenderObject();
	void					FreeRenderObejct();

	void					AttachRenderObject( GdsRenderObjectPtr pRenderObject , int iRenderStateGroupID );
	void					DetachRenderObject( GdsRenderObjectPtr pRenderObject );

	void					AddRenderStateGroup( GdsRenderStateGroupPtr renderstategroup , int iRenderStateGroupID );

	void					Render( LPDIRECT3DDEVICE9 device ){ vRender( device ); }

	size_t					GetRenderObjectCount(){ return m_RenderFrame.size(); }

protected:

	virtual	void			vRender( LPDIRECT3DDEVICE9 device );

};

typedef boost::shared_ptr< GdsRenderFrame > GdsRenderFramePtr;

#endif