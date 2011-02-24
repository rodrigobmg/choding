#ifndef _SNOWBOARD_SCENENODE_BASE_
#define _SNOWBOARD_SCENENODE_BASE_

#include "../Base/GdsObject.h"

class GdsNode : public GdsObject{
		
protected:
	GdsNode*							m_pParentNode;

	typedef	 std::list< GdsNode* >	CHILDLIST;
	CHILDLIST							m_listChildNode;

	virtual HRESULT vUpdate( float fElapsedtime );	


public:
	GdsNode();
	virtual ~GdsNode();

	HRESULT			Update( float fElapsedtime );
	GdsNode*		GetParent();
	void			SetParent( GdsNode* pNode );
	GdsNode*		GetAt( unsigned int index );
	
	virtual HRESULT AttachChild( GdsNode* pNode );
	virtual HRESULT DetachChild( GdsNode* pNode );
	virtual HRESULT	RemoveAllChild();

};

typedef boost::shared_ptr< GdsNode >	GdsNodePtr;

#endif