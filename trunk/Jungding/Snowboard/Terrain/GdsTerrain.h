#ifndef _GDS_CORE_TERRAIN_H_
#define _GDS_CORE_TERRAIN_H_

class GdsTerrain
{
	
	// index-templet for slod
	typedef		std::vector< GdsIndexBufferPtr > INDEX_CONTAINER;
	std::vector< INDEX_CONTAINER > m_LODIBTemplet;

	int							m_iVertexPerNode;
	int							m_ixheight;
	int							m_izheight;
	int							m_iMaxLOD;
	int							m_iLodRate;

	int							m_ilod;
	int							m_idir;
	// root tile for quad-tree
	TILE*						m_pRootTile;
	void						genIndex( TILE* tile );
	void						build( TILE* tile , GDSVERTEX* pVB );

	bool						createTempletIB();

protected:			

	virtual void				vClear();

public:

	GdsTerrain();
	virtual ~GdsTerrain();

	//void						Build(){ build( m_pRootTile ); }

	void						Clear(){ vClear(); }

	void						SetLODRate( int ilevel ){ m_iLodRate = ilevel; }
	int							GetLODRate(){ return m_iLodRate; }
	
	void						SetVertexPerPatch( int ivalue ){ m_iVertexPerNode = ivalue; }
	int							GetVertexPerPatch(){ return m_iVertexPerNode; }

	bool						MakeHeightMap();

	void						Update( float fElapsedtime );
};

#define TERRAIN	singleton_holder< GdsTerrain >::instance()

#endif
