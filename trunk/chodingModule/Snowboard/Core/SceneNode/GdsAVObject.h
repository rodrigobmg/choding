#ifndef _SNOWBOARD_SCENENODE_CHILDMESH_
#define _SNOWBOARD_SCENENODE_CHILDMESH_

#include "../Base/GdsObject.h"

class GdsNode;

class GdsAVObject : public GdsObject{		

protected:
	
	GdsNode*		m_pParentNode;


	virtual	void	vInitGeometry();
	virtual void	vRender();
	virtual HRESULT vUpdate( float fElapsedtime );	

public:
	GdsAVObject();
	virtual ~GdsAVObject();


	HRESULT			Update( float fElapsedtime );
	GdsNode*		GetParent();
	void			SetParent( GdsNode* pNode );
	
	
};

typedef boost::shared_ptr< GdsAVObject >	GdsAVObjectPtr;

#endif