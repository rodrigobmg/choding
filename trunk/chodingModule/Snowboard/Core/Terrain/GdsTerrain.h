#ifndef _GDS_CORE_TERRAIN_H_
#define _GDS_CORE_TERRAIN_H_

#include "..\Base\GdsObject.h"
#include "..\SceneNode\GdsNode.h"
#include "..\..\System\Singleton\Singleton.h"
#include "..\Resource\Type\GdsIndexBuffer.h"
#include "..\Resource\Type\GdsVertexBuffer.h"

class GdsTerrain : public GdsObject
{
	struct TRIANGLE
	{
		// direction LEFT:0 , NEAR:1 , RIGHT:2 , FAR:3
		//		  /| p2
		//center / |
		//		/  |
		//   p1/---|  corner
		//  order to triangle index p1 , corner , p2
		enum DIR
		{
			NONE = 0,
			WEST,
			SOUTH,
			EAST,
			NORTH,
		};

		D3DXVECTOR3 p1 , corner , p2 , center;
		int dir,m_iLodlv;
		TRIANGLE*	m_pLeft;
		TRIANGLE*	m_pRight;

		TRIANGLE();
		~TRIANGLE();

		void calcDir( D3DXVECTOR3& center , D3DXVECTOR3& corner , int& dir );
		void split( TRIANGLE* tri , int idir , int lodlv , int iLodLimit );
		void genIndexTemplet( std::vector< D3DXVECTOR3 >& vecList , int ilodlv , int icrackDir );
		void GetVertex( GDSVERTEX* tile , int x , int z , GDSVERTEX& vertex );
		void GetIndex( D3DXVECTOR3& vertex , int& index );
	};


	// tile
	struct TILE
	{
		// 33 * 33
		int						m_iLOD;
		int						m_iMaxLOD;
		TILE*					m_pParent;
		TILE*					m_pChild[4];
		D3DXVECTOR3				m_minPos;
		D3DXVECTOR3				m_maxPos;
		D3DXVECTOR3				m_cenPos;
		GdsVertexBufferPtr		m_pVB;
		TILE()
		{
			m_pParent = NULL;
			m_pVB		= GdsVertexBufferPtr( (GdsVertexBuffer*)NULL );
			m_iLOD = 0;
			m_iMaxLOD = 3;						
			m_maxPos = D3DXVECTOR3( 0.f , 0.f , 0.f );
			m_minPos = D3DXVECTOR3( 0.f , 0.f , 0.f );
			m_cenPos = D3DXVECTOR3( 0.f , 0.f , 0.f );

			for ( int i=0 ;i < 4 ; i++)
			{
				m_pChild[i] = NULL;
			}
		}

		~TILE()
		{
			for ( int i=0 ;i < 4 ; i++)
			{
				SAFE_DELETE( m_pChild[i] );
			}			
		}
	};

	// index-templet for slod
	//std::vector<GdsIndexBufferPtr>	m_LODIBTemplet;
	typedef		std::vector< GdsIndexBufferPtr > INDEX_CONTAINER;
	std::vector< INDEX_CONTAINER > m_LODIBTemplet;

	int							m_iVertexPerNode;
	int							m_ixheight;
	int							m_izheight;
	int							m_iMaxLOD;
	int							m_iLodRate;
	// roottile for quad-tree
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
