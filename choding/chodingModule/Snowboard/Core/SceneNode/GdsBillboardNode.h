#ifndef _SNOWBOARD_SCENENODE_BILLBOARD_H_
#define _SNOWBOARD_SCENENODE_BILLBOARD_H_

#include "GdsNode.h"

class GdsBillboardNode : public GdsNode 
{	
private:

public:
	GdsBillboardNode();
	virtual ~GdsBillboardNode();

protected:
	virtual	void	vInitGeometry( float fElapsedtime );
	virtual void	vRender( float fElapsedtime );
	virtual void	vClear();
};

typedef boost::shared_ptr< GdsBillboardNode >	GdsBillboardNodePtr;

#endif