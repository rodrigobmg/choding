#include "GdsFrustum.h"

GdsFrustum::GdsFrustum( float fLeft , float fRight , float fBottom , float fTop , float fNear , float fFar , bool bOrtho /*= false*/ )
{
	SetName( OBJECT_FRUSTUM );

	m_bOrtho = bOrtho;
	
	m_fZnear = fNear;
	m_fFar = fFar;
	m_fLeft = fLeft;
	m_fRight = fRight;
	m_fBottom = fBottom;
	m_fTop = fTop;
}