#ifndef _GDS_CORE_TRANSFORM_H_
#define _GDS_CORE_TRANSFORM_H_

#include "../Base/GdsObject.h"

class GdsTransform : public GdsObject
{
public:
	GdsTransform();
	virtual ~GdsTransform();

	//���߿� ���� ��İ� ���ͷ� �ٲܰ���
	D3DXMATRIXA16	m_matRotate;
	D3DXVECTOR3		m_vecTranslate;
	float			m_fScale;

	void			MakeIdentity();
	bool operator!=(const GdsTransform &xform) const;
	GdsTransform operator*(const GdsTransform &xform) const;

};

#endif