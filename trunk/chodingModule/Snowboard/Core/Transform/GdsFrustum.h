#ifndef _GDS_CORE_TRANSFORM_FRUSTUM_H_
#define _GDS_CORE_TRANSFORM_FRUSTUM_H_

#include "../Base/GdsObject.h"

class GdsFrustum : public GdsObject
{
public:

	DeclareBoostPool

	GdsFrustum(){ SetName( OBJECT_FRUSTUM ); m_bOrtho = false; }
	GdsFrustum( float fLeft , float fRight , float fBottom , float fTop , float fNear , float fFar , bool bOrtho = false);
	
	float		m_fZnear;
	float		m_fFar;

	float		m_fLeft;
	float		m_fRight;
	float		m_fTop;
	float		m_fBottom;


	bool		m_bOrtho;
};

#endif