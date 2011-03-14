#include "GdsTransform.h"

GdsTransform::GdsTransform()
{
}

GdsTransform::~GdsTransform()
{	
}

void GdsTransform::MakeIdentity()
{
	D3DXMatrixIdentity( &m_matRotate );//.MakeIdentity();
	
	m_vecTranslate.x = 0.f;
	m_vecTranslate.y = 0.f;
	m_vecTranslate.z = 0.f;

	m_fScale = 1.0f;
}

bool GdsTransform::operator!=(const GdsTransform&xform) const
{
	return true;
// 		m_Rotate != xform.m_Rotate ||
// 		m_Translate != xform.m_Translate ||
// 		m_fScale != xform.m_fScale;
}