#ifndef _GDS_CORE_TRANSFORM_FRUSTUM_H_
#define _GDS_CORE_TRANSFORM_FRUSTUM_H_

#include "../Base/GdsObject.h"

class GdsFrustum : public GdsObject
{

	D3DXVECTOR3 m_vtx[8];
	D3DXPLANE	m_plane[6];
public:

	float		m_fZnear;
	float		m_fFar;

	float		m_fLeft;
	float		m_fRight;
	float		m_fTop;
	float		m_fBottom;

	bool		m_bOrtho;

	GdsFrustum(){ SetName( OBJECT_FRUSTUM ); m_bOrtho = false; }
	GdsFrustum( float fLeft , float fRight , float fBottom , float fTop , float fNear , float fFar , bool bOrtho = false);
	
	void		UpdatePlane( const D3DXMATRIX& viewprojMat );
	bool		VertexIsInFrustum( const D3DXVECTOR3& v );
	bool		SphereIsInFrustum( const D3DXVECTOR3& v, float fradius ) ;
};

#endif