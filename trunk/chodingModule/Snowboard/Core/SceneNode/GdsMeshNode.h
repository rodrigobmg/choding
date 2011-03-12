#ifndef _SNOWBOARD_SCENENODE_MESH_
#define _SNOWBOARD_SCENENODE_MESH_

#include "GdsNode.h"

class GdsMeshNode : public GdsNode{		

protected:

	virtual	void				vInitGeometry();
	virtual void				vRender();
	virtual void				vClear();

public:
	GdsMeshNode();
	virtual ~GdsMeshNode();

	void						SetVB( LPDIRECT3DVERTEXBUFFER9 vb );
	LPDIRECT3DVERTEXBUFFER9		GetVB();

	void						SetFVF( DWORD flag );
	DWORD						GetFVF();

	void						SetTexture( LPDIRECT3DTEXTURE9 texture );
	LPDIRECT3DTEXTURE9			GetTexture();

	void						SetPrimitive( UINT uPrimitive );
	UINT						GetPrimitive();

};

typedef boost::shared_ptr<GdsMeshNode>	GdsMeshNodePtr;

#endif