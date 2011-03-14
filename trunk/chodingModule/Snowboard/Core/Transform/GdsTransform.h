#ifndef _GDS_CORE_TRANSFORM_H_
#define _GDS_CORE_TRANSFORM_H_

#include "../Base/GdsObject.h"
#include "GdsMatrix.h"
#include "GdsVector.h"

class GdsTransform : public GdsObject
{
public:

 	GdsMatrix		m_Rotate;
 	GdsVector3		m_Translate;
 	float			m_fScale;

	bool operator!=(const GdsTransform &xform) const;
	inline GdsTransform GdsTransform::operator*(const GdsTransform &xform) const
	{
		GdsTransform res;

		res.m_fScale = m_fScale * xform.m_fScale;
		res.m_Rotate = m_Rotate * xform.m_Rotate;
		res.m_Translate = m_Translate + m_fScale * (m_Rotate * xform.m_Translate);

		return res;
	}
	inline GdsVector3 GdsTransform::operator*(const GdsVector3 &kPoint) const
	{
		return (((m_Rotate * kPoint) * m_fScale) + m_Translate);
	}

	void		MakeIdentity();
	void		Invert(GdsTransform& kDest) const;
};

#endif