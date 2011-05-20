#ifndef _GDS_CORE_TERRAIN_H_
#define _GDS_CORE_TERRAIN_H_

#include "..\Base\GdsObject.h"
#include "..\SceneNode\GdsNode.h"
#include "..\..\System\Singleton\Singleton.h"

class GdsTerrain : public GdsObject
{
	// index-templet for slod
	LPDIRECT3DINDEXBUFFER9*		m_pIB;

	int							m_iVertexPerPatch;
	int							m_ixheight;
	int							m_izheight;
	int							m_iMaxLOD;

	void						createIB();
	void						createVB();

	// tile
	typedef	std::vector< GdsRenderObject >	TILE;
	TILE						m_vecTile;

protected:			

	virtual void				vClear();

public:

	GdsTerrain();
	virtual ~GdsTerrain();

	void						Clear(){ vClear(); }

	void						SetMaxLOD( int ilevel ){ m_iMaxLOD = ilevel; }
	int							GetMaxLOD(){ return m_iMaxLOD; }
	
	void						SetVertexPerPatch( int ivalue ){ m_iVertexPerPatch = ivalue; }
	int							GetVertexPerPatch(){ return m_iVertexPerPatch; }

	bool						MakeHeightMap( GdsNodePtr pNode );

};

#define TERRAIN	singleton_holder< GdsTerrain >::instance()

#endif