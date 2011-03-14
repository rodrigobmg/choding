#ifndef _GDS_CORE_TRANSFORM_VECTOR_H_
#define _GDS_CORE_TRANSFORM_VECTOR_H_

#include "../Base/GdsObject.h"

class GdsVector3 : public GdsObject
{
public:
	float x , y , z;

	GdsVector3();
	GdsVector3( float x_ , float y_ , float z_ ) { x = x_ ; y = y_ ; z = z_; }
	virtual ~GdsVector3();

	float&	operator[] ( int i )
	{
		float* p = &x;
		return (float&)p[i];
	}

	const float&	operator[] ( int i ) const
	{
		const float* p = &x;
		return (float&)p[i];
	}

	bool	operator== ( const GdsVector3& r ) const;
	bool	operator!= ( const GdsVector3& r ) const;

	GdsVector3 operator+ ( const GdsVector3& r ) const;
	GdsVector3 operator- ( const GdsVector3& r ) const;
	GdsVector3 operator* ( const float fvalue ) const;
	GdsVector3 operator/ ( const float fvalue ) const;
	
	GdsVector3 operator- () const;

	static GdsVector3 ComponentProduct (const GdsVector3& p0, const GdsVector3& p1);

	GdsVector3& operator+= (const GdsVector3& pt);
	GdsVector3& operator-= (const GdsVector3& pt);
	GdsVector3& operator*= (float fScalar);
	GdsVector3& operator/= (float fScalar);

	float Length () const;
	float SqrLength () const;
	float Dot (const GdsVector3& pt) const;
	float Unitize ();
	GdsVector3 Cross (const GdsVector3& pt) const;
	GdsVector3 UnitCross (const GdsVector3& pt) const;


	static const GdsVector3 ZERO;
	static const GdsVector3 UNIT_X;
	static const GdsVector3 UNIT_Y;
	static const GdsVector3 UNIT_Z;
	static const GdsVector3 UNIT_ALL;

};


#endif