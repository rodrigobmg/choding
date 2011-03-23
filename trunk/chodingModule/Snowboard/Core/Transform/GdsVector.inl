inline GdsVector3::GdsVector3()
{
	SetName( OBJECT_VECTOR3 );
}
//---------------------------------------------------------------------------
inline GdsVector3::GdsVector3(float fX, float fY, float fZ)
{
	SetName( OBJECT_VECTOR3 );
	x = fX;
	y = fY;
	z = fZ;
}
//---------------------------------------------------------------------------
inline const float& GdsVector3::operator[] (int i) const
{
	// warning:  safe for 4-byte aligned class member data
	const float* base = &x;
	return (float&) base[i];
}
//---------------------------------------------------------------------------
inline float& GdsVector3::operator[] (int i)
{
	// warning:  safe for 4-byte aligned class member data
	float* base = &x;
	return (float&) base[i];
}
//---------------------------------------------------------------------------
inline bool GdsVector3::operator== (const GdsVector3& kPt) const
{
	return (x == kPt.x && y == kPt.y && z == kPt.z);
}
//---------------------------------------------------------------------------
inline bool GdsVector3::operator!= (const GdsVector3& pt) const
{
	return !(*this == pt);
}
//---------------------------------------------------------------------------
inline GdsVector3 GdsVector3::operator+ (const GdsVector3& kPt) const
{
	return GdsVector3(x+kPt.x,y+kPt.y,z+kPt.z);
}
//---------------------------------------------------------------------------
inline GdsVector3 GdsVector3::operator- (const GdsVector3& kPt) const
{
	return GdsVector3(x-kPt.x,y-kPt.y,z-kPt.z);
}
//---------------------------------------------------------------------------
inline float GdsVector3::operator* (const GdsVector3& kPt) const
{
	return x*kPt.x+y*kPt.y+z*kPt.z;
}
//---------------------------------------------------------------------------
inline GdsVector3 GdsVector3::operator* (float fScalar) const
{
	return GdsVector3(fScalar*x,fScalar*y,fScalar*z);
}
//---------------------------------------------------------------------------
inline GdsVector3 GdsVector3::operator/ (float fScalar) const
{
	float fInvScalar = 1.0f/fScalar;
	return GdsVector3(fInvScalar*x,fInvScalar*y,fInvScalar*z);
}
//---------------------------------------------------------------------------
inline GdsVector3 GdsVector3::operator- () const
{
	return GdsVector3(-x,-y,-z);
}
//---------------------------------------------------------------------------
inline GdsVector3 operator* (float fScalar, const GdsVector3& kPt)
{
	return GdsVector3(fScalar*kPt.x,fScalar*kPt.y,fScalar*kPt.z);
}
//---------------------------------------------------------------------------
inline GdsVector3& GdsVector3::operator+= (const GdsVector3& kPt)
{
	x += kPt.x;
	y += kPt.y;
	z += kPt.z;
	return *this;
}
//---------------------------------------------------------------------------
inline GdsVector3& GdsVector3::operator-= (const GdsVector3& kPt)
{
	x -= kPt.x;
	y -= kPt.y;
	z -= kPt.z;
	return *this;
}
//---------------------------------------------------------------------------
inline GdsVector3& GdsVector3::operator*= (float fScalar)
{
	x *= fScalar;
	y *= fScalar;
	z *= fScalar;
	return *this;
}
//---------------------------------------------------------------------------
inline GdsVector3& GdsVector3::operator/= (float fScalar)
{
	float fInvScalar = 1.0f/fScalar;
	x *= fInvScalar;
	y *= fInvScalar;
	z *= fInvScalar;
	return *this;
}
//---------------------------------------------------------------------------
inline GdsVector3 GdsVector3::ComponentProduct(const GdsVector3& kP0,
										   const GdsVector3& kP1)
{
	return GdsVector3(kP0.x*kP1.x,kP0.y*kP1.y,kP0.z*kP1.z);
}
//---------------------------------------------------------------------------
inline float GdsVector3::Length() const
{
	return sqrt(x*x+y*y+z*z);
}
//---------------------------------------------------------------------------
inline float GdsVector3::SqrLength() const
{
	return x*x+y*y+z*z;
}
//---------------------------------------------------------------------------
inline float GdsVector3::Dot(const GdsVector3& kPt) const
{
	return x*kPt.x+y*kPt.y+z*kPt.z;
}
//---------------------------------------------------------------------------
inline float GdsVector3::Normalize()
{
	float fLength = Length();

	if ( fLength > 1e-06f )
	{
		float fRecip = 1.0f/fLength;
		x *= fRecip;
		y *= fRecip;
		z *= fRecip;
	}
	else
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		fLength = 0.0f;
	}

	return fLength;
}
//---------------------------------------------------------------------------
inline GdsVector3 GdsVector3::Cross(const GdsVector3& kPt) const
{
	return GdsVector3(y*kPt.z-z*kPt.y,z*kPt.x-x*kPt.z,x*kPt.y-y*kPt.x);
}
//---------------------------------------------------------------------------
inline GdsVector3 GdsVector3::UnitCross(const GdsVector3& kPt) const
{
	GdsVector3 cross(y*kPt.z-z*kPt.y,z*kPt.x-x*kPt.z,x*kPt.y-y*kPt.x);
	float fLength = cross.Length();
	if ( fLength > 1e-06f )
		return cross/fLength;
	else
		return GdsVector3(0.0f,0.0f,0.0f);
}

//---------------------------------------------------------------------------
inline GdsVector3 GdsVector3::Perpendicular() const
{
	if(fabs(x) < fabs(y))
	{
		if(fabs(z) < fabs(x))
			return UnitCross(GdsVector3::UNIT_Z);
		else
			return UnitCross(GdsVector3::UNIT_X);
	}
	else
	{
		if(fabs(z) < fabs(y))
			return UnitCross(GdsVector3::UNIT_Z);
		else
			return UnitCross(GdsVector3::UNIT_Y);
	}
}