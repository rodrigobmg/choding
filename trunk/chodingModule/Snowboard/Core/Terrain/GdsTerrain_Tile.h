#ifndef _GDS_CORE_TERRAIN_TILE_H_
#define _GDS_CORE_TERRAIN_TILE_H_

#include "..\Resource\Type\GdsVertexBuffer.h"

// tile
class TILE
{

public:
	// 33 * 33
	int						m_iLOD;
	int						m_iMaxLOD;
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
