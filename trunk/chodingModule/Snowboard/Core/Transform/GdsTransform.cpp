#include "GdsTransform.h"

//ImplementBoostPool( GdsTransform )

void GdsTransform::MakeIdentity()
{
	m_Rotate.MakeIdentity();
	m_Translate = GdsVector3::ZERO;
	m_fScale = 1.0f;
}

//---------------------------------------------------------------------------
bool GdsTransform::operator!=(const GdsTransform &xform) const
{
	return 
		m_Rotate != xform.m_Rotate ||
		m_Translate != xform.m_Translate ||
		m_fScale != xform.m_fScale;
}

//---------------------------------------------------------------------------
void GdsTransform::Invert(GdsTransform& kDest) const
{
	kDest.m_Rotate = m_Rotate.Transpose();
	kDest.m_fScale = 1.0f / m_fScale;
	kDest.m_Translate = kDest.m_fScale * (kDest.m_Rotate * -m_Translate);
}
//---------------------------------------------------------------------------
/*
void GdsTransform::Interpolate(
							  float fAmount, 
							  const GdsTransform& kStart,
							  const GdsTransform& kFinish,
							  GdsTransform& kOutput)
{
	if (fAmount <= 0.f)
	{
		kOutput = kStart;
	}
	else if (fAmount >= 1.f)
	{
		kOutput = kFinish;
	}
	else
	{
		kOutput.m_fScale = 
			NiLerp(fAmount, 
			kStart.m_fScale, 
			kFinish.m_fScale);

		kOutput.m_Translate = 
			NiLerp(fAmount, 
			kStart.m_Translate, 
			kFinish.m_Translate);

		NiQuaternion kStartQuat;
		kStartQuat.FromRotation(kStart.m_Rotate);
		NiQuaternion kFinishQuat;
		kFinishQuat.FromRotation(kFinish.m_Rotate);
		NiQuaternion kResultQuat(NiQuaternion::Slerp(
			fAmount, 
			kStartQuat,
			kFinishQuat));
		kResultQuat.ToRotation(kOutput.m_Rotate);
	}
}*/
