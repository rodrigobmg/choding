#ifndef _SNOWBOARD_SCENENODE_BASE_
#define _SNOWBOARD_SCENENODE_BASE_

#include "../Base\GdsObject.h"

class GdsNode : public GdsObject , public boost::enable_shared_from_this< GdsNode >
{
		
protected:	

	typedef boost::shared_ptr< GdsNode >	GdsNodePtr;

	GdsNodePtr							m_pParentNode;

	typedef	 std::list< GdsNodePtr >	CHILDLIST;
	CHILDLIST							m_listChildNode;

	virtual	void	vInitGeometry();
	virtual void	vRender();
	virtual void	vClear();

public:
	GdsNode();
	virtual ~GdsNode();

	boost::shared_ptr< GdsNode > shared_ptr_this()
	{
		return shared_from_this();
	}

	HRESULT			Update( float fElapsedtime );
	GdsNodePtr		GetParent();
	void			SetParent( GdsNodePtr pNode );
	GdsNodePtr		GetAt( unsigned int index );
	
	virtual HRESULT AttachChild( GdsNodePtr pNode );
	virtual HRESULT DetachChild( GdsNodePtr pNode );
	virtual HRESULT	RemoveAllChild();

};

typedef boost::shared_ptr< GdsNode >	GdsNodePtr;

#endif