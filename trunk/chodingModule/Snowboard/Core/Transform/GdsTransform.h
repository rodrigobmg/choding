#ifndef _GDS_CORE_TRANSFORM_H_
#define _GDS_CORE_TRANSFORM_H_

#include "../Base/GdsObject.h"
#include "GdsMatrix.h"
#include "GdsVector.h"

class GdsTransform : public GdsObject
{
public:
	GdsTransform();
	virtual ~GdsTransform();

	//���߿� ���� ��İ� ���ͷ� �ٲܰ���
	GdsMatrix		m_matRotate;
	GdsVector3		m_vecTranslate;
	float			m_fScale;

	void			MakeIdentity();
	bool operator!=(const GdsTransform &xform) const;
	GdsTransform operator*(const GdsTransform &xform) const;
	GdsVector3	 operator*(const GdsVector3 &xVec ) const;
};

#endif