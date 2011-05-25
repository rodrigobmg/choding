#ifndef _GDS_CORE_TERRAIN_H_
#define _GDS_CORE_TERRAIN_H_

#include "..\Base\GdsObject.h"
#include "..\SceneNode\GdsNode.h"
#include "..\..\System\Singleton\Singleton.h"

class GdsTerrain : public GdsObject
{
	// index-templet for slod
	LPDIRECT3DINDEXBUFFER9**	m_ppIB;

	int							m_iVertexPerNode;
	int							m_ixheight;
	int							m_izheight;
	int							m_iMaxLOD;
	int							m_iLodRate;
	
	// tile
	struct TILE
	{
		// 16 * 16
		GDSVERTEX*				m_pVertex;
		int						m_iLOD;
		int						m_iMaxLOD;
		TILE*					m_pParent;
		TILE*					m_pChild[4];
		TILE*					m_pNeighbor[4];
		D3DXVECTOR3				m_minPos;
		D3DXVECTOR3				m_maxPos;
		D3DXVECTOR3				m_cenPos;
		GdsRenderObjectPtr		m_RenderToken;
		TILE()
		{
			m_pVertex = NULL;
			m_pParent = NULL;
			m_RenderToken = GdsRenderObjectPtr( (GdsRenderObject*)NULL );
			m_iLOD = 0;
			m_iMaxLOD = 3;						
			m_maxPos = D3DXVECTOR3( 0.f , 0.f , 0.f );
			m_minPos = D3DXVECTOR3( 0.f , 0.f , 0.f );
			m_cenPos = D3DXVECTOR3( 0.f , 0.f , 0.f );

			for ( int i=0 ;i < 4 ; i++)
			{
				m_pChild[i] = NULL;
				m_pNeighbor[i] = NULL;
			}
		}

		~TILE()
		{
			SAFE_DELETE( m_pVertex );
			for ( int i=0 ;i < 4 ; i++)
			{
				SAFE_DELETE( m_pChild[i] );
			}			
		}
	};

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