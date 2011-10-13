#ifndef _GDS_CORE_TERRAIN_TILE_H_
#define _GDS_CORE_TERRAIN_TILE_H_

// tile
struct TILE
{
	// 33 * 33
	float					m_fRadius;
	int						m_iLOD;
	int						m_iCrackLOD;
	TILE*					m_pParent;
	TILE*					m_pChild[4];
	D3DXVECTOR3				m_minPos;
	D3DXVECTOR3				m_maxPos;
	D3DXVECTOR3				m_cenPos;
	GdsVertexBufferPtr		m_pVB;


	TILE();
	~TILE();
};


#endif
