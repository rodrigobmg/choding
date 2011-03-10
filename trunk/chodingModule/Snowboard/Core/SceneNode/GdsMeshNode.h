#ifndef _SNOWBOARD_SCENENODE_MESH_
#define _SNOWBOARD_SCENENODE_MESH_

#include "GdsNode.h"

class GdsMeshNode : public GdsNode{		

protected:

	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DTEXTURE9			m_pTexture;
	DWORD						m_dFVF;
	UINT						m_uPrimitive;

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