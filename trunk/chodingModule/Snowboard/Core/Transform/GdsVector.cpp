#include "GdsVector.h"


const GdsVector3 GdsVector3::ZERO(0.0f,0.0f,0.0f);
const GdsVector3 GdsVector3::UNIT_X(1.0f,0.0f,0.0f);
const GdsVector3 GdsVector3::UNIT_Y(0.0f,1.0f,0.0f);
const GdsVector3 GdsVector3::UNIT_Z(0.0f,0.0f,1.0f);
const GdsVector3 GdsVector3::UNIT_ALL(1.0f,1.0f,1.0f);


GdsVector3::GdsVector3()
{
}

GdsVector3::~GdsVector3()
{	
}

inline bool GdsVector3::operator== ( const GdsVector3& r ) const
{
	return (x == r.x && y == r.y && z == r.z);
}

inline bool GdsVector3::operator!= ( const GdsVector3& r ) const
{
	return !(*this == r);
}

inline GdsVector3 GdsVector3::operator+ ( const GdsVector3& r ) const
{
	return GdsVector3( x+r.x , y+r.y , z+r.z );
}

inline GdsVector3 GdsVector3::operator- ( const GdsVector3& r ) const
{
	return GdsVector3( x-r.x , y-r.y , z-r.z );
}

inline GdsVector3 GdsVector3::operator* ( const float fvalue ) const
{
	return GdsVector3( x*fvalue , y*fvalue , z*fvalue );
}

inline GdsVector3 GdsVector3::operator/ ( const float fvalue ) const
{
	return GdsVector3( x/fvalue , y/fvalue , z/fvalue );
}

inline GdsVector3 GdsVector3::operator- () const
{
	return GdsVector3( -x , -y , -z );
}

inline GdsVector3& GdsVector3::operator+= (const GdsVector3& pt)
{
	x += pt.x;
	y += pt.y;
	z += pt.z;
	return *this;
}

inline GdsVector3& GdsVector3::operator-= (const GdsVector3& pt)
{
	x -= pt.x;
	y -= pt.y;
	z -= pt.z;
	return *this;
}

inline GdsVector3& GdsVector3::operator*= (float fScalar)
{
	x *= fScalar;
	y *= fScalar;
	z *= fScalar;
	return *this;
}

inline GdsVector3& GdsVector3::operator/= (float fScalar)
{
	float fInvScalar = 1.0f/fScalar;
	x *= fInvScalar;
	y *= fInvScalar;
	z *= fInvScalar;
	return *this;
}

inline GdsVector3 GdsVector3::ComponentProduct (const GdsVector3& p0,
											const GdsVector3& p1)
{
	return GdsVector3(p0.x*p1.x,p0.y*p1.y,p0.z*p1.z);
}

inline float GdsVector3::Length () const
{
	return (float)sqrt(x*x+y*y+z*z);
}

inline float GdsVector3::SqrLength () const
{
	return x*x+y*y+z*z;
}

inline float GdsVector3::Dot (const GdsVector3& pt) const
{
	return x*pt.x+y*pt.y+z*pt.z;
}

inline float GdsVector3::Unitize ()
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

inline GdsVector3 GdsVector3::Cross (const GdsVector3& pt) const
{
	return GdsVector3(y*pt.z-z*pt.y,z*pt.x-x*pt.z,x*pt.y-y*pt.x);
}

inline GdsVector3 GdsVector3::UnitCross (const GdsVector3& pt) const
{
	GdsVector3 cross(y*pt.z-z*pt.y,z*pt.x-x*pt.z,x*pt.y-y*pt.x);
	float fLength = cross.Length();
	if ( fLength > 1e-06f )
		return cross/fLength;
	else
		return GdsVector3(0.0f,0.0f,0.0f);
}