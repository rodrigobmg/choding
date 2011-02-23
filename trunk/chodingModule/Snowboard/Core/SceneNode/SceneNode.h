#ifndef _SNOWBOARD_SCENENODE_BASE_
#define _SNOWBOARD_SCENENODE_BASE_

#include "../Base/Snow.h"

class SceneNode : public CSnow{
		
protected:
	SceneNode*							m_pParentNode;

	typedef	 std::list< SceneNode* >	CHILDLIST;
	CHILDLIST							m_listChildNode;

	virtual HRESULT vUpdate( float fElapsedtime );	


public:
	SceneNode();
	virtual ~SceneNode();

	HRESULT			Update( float fElapsedtime );
	SceneNode*		GetParent();
	void			SetParent( SceneNode* pNode );
	SceneNode*		GetAt( unsigned int index );
	
	HRESULT			AttachChild( SceneNode* pNode );
	HRESULT			DetachChild( SceneNode* pNode );
	HRESULT			RemoveAllChild();

};

#endif