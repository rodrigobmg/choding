#ifndef _GDS_CORE_TRANSFORM_VECTOR_H_
#define _GDS_CORE_TRANSFORM_VECTOR_H_

#include "../Base/GdsObject.h"

class GdsVector3 : public GdsObject
{
public:

	float x , y , z;

	static const GdsVector3 ZERO;
	static const GdsVector3 UNIT_X;
	static const GdsVector3 UNIT_Y;
	static const GdsVector3 UNIT_Z;
	static const GdsVector3 UNIT_ALL;

	inline GdsVector3();
	inline GdsVector3 (float fX, float fY, float fZ);

	inline const float& operator[] (int i) const;
	inline float& operator[] (int i);

	inline bool operator== (const GdsVector3& pt) const;
	inline bool operator!= (const GdsVector3& pt) const;

	inline GdsVector3 operator+ (const GdsVector3& pt) const;
	inline GdsVector3 operator- (const GdsVector3& pt) const;
	inline float operator* (const GdsVector3& pt) const;
	inline GdsVector3 operator* (float fScalar) const;
	inline GdsVector3 operator/ (float fScalar) const;
	inline GdsVector3 operator- () const;

	static GdsVector3 ComponentProduct (const GdsVector3& p0, const GdsVector3& p1);

	inline GdsVector3& operator+= (const GdsVector3& pt);
	inline GdsVector3& operator-= (const GdsVector3& pt);
	inline GdsVector3& operator*= (float fScalar);
	inline GdsVector3& operator/= (float fScalar);

	float Length () const;
	inline float SqrLength () const;
	inline float Dot (const GdsVector3& pt) const;
	inline float Normalize ();
	inline GdsVector3 Cross (const GdsVector3& pt) const;
	GdsVector3 UnitCross (const GdsVector3& pt) const;

	// This finds one of the infinite set of perpendicular vectors
	inline GdsVector3 Perpendicular () const;

	//DeclareBoostPool(GdsVector3)
};

#include "GdsVector.inl"

#endif