#include "GdsMatrix.h"

#include <cfloat>
#include <climits>
#include <cmath>

const GdsMatrix GdsMatrix::ZERO(
								GdsVector3(0.0f, 0.0f, 0.0f),
								GdsVector3(0.0f, 0.0f, 0.0f),
								GdsVector3(0.0f, 0.0f, 0.0f));

const GdsMatrix GdsMatrix::IDENTITY(
									GdsVector3(1.0f, 0.0f, 0.0f),
									GdsVector3(0.0f, 1.0f, 0.0f),
									GdsVector3(0.0f, 0.0f, 1.0f));


GdsMatrix::GdsMatrix (const GdsVector3& col0, const GdsVector3& col1, 
					  const GdsVector3& col2 )
{
	SetCol( 0, col0 );
	SetCol( 1, col1 );
	SetCol( 2, col2 );
}
//---------------------------------------------------------------------------
void GdsMatrix::MakeIdentity ()
{
	m_pEntry[0][0] = 1.0f;
	m_pEntry[0][1] = 0.0f;
	m_pEntry[0][2] = 0.0f;
	m_pEntry[1][0] = 0.0f;
	m_pEntry[1][1] = 1.0f;
	m_pEntry[1][2] = 0.0f;
	m_pEntry[2][0] = 0.0f;
	m_pEntry[2][1] = 0.0f;
	m_pEntry[2][2] = 1.0f;
}
//---------------------------------------------------------------------------
void GdsMatrix::MakeDiagonal (float fDiag0, float fDiag1, float fDiag2)
{
	m_pEntry[0][0] = fDiag0;
	m_pEntry[0][1] = 0.0f;
	m_pEntry[0][2] = 0.0f;
	m_pEntry[1][0] = 0.0f;
	m_pEntry[1][1] = fDiag1;
	m_pEntry[1][2] = 0.0f;
	m_pEntry[2][0] = 0.0f;
	m_pEntry[2][1] = 0.0f;
	m_pEntry[2][2] = fDiag2;
}
//---------------------------------------------------------------------------
void GdsMatrix::MakeXRotation (float fAngle)
{
	float sn, cs;
	sn = sin( fAngle );
	cs = cos( fAngle );
	//NiSinCos(fAngle, sn, cs);

	m_pEntry[0][0] = 1.0f;
	m_pEntry[0][1] = 0.0f;
	m_pEntry[0][2] = 0.0f;
	m_pEntry[1][0] = 0.0f;
	m_pEntry[1][1] = cs;
	m_pEntry[1][2] = sn;
	m_pEntry[2][0] = 0.0f;
	m_pEntry[2][1] = -sn;
	m_pEntry[2][2] = cs;
}
//---------------------------------------------------------------------------
void GdsMatrix::MakeYRotation (float fAngle)
{
	float sn, cs;
	sn = sin( fAngle );
	cs = cos( fAngle );
	//NiSinCos(fAngle, sn, cs);

	m_pEntry[0][0] = cs;
	m_pEntry[0][1] = 0.0f;
	m_pEntry[0][2] = -sn;
	m_pEntry[1][0] = 0.0f;
	m_pEntry[1][1] = 1.0f;
	m_pEntry[1][2] = 0.0f;
	m_pEntry[2][0] = sn;
	m_pEntry[2][1] = 0.0f;
	m_pEntry[2][2] = cs;
}
//---------------------------------------------------------------------------
void GdsMatrix::MakeZRotation (float fAngle)
{
	float sn, cs;
	sn = sin( fAngle );
	cs = cos( fAngle );
	//NiSinCos(fAngle, sn, cs);

	m_pEntry[0][0] = cs;
	m_pEntry[0][1] = sn;
	m_pEntry[0][2] = 0.0f;
	m_pEntry[1][0] = -sn;
	m_pEntry[1][1] = cs;
	m_pEntry[1][2] = 0.0f;
	m_pEntry[2][0] = 0.0f;
	m_pEntry[2][1] = 0.0f;
	m_pEntry[2][2] = 1.0f;
}
//---------------------------------------------------------------------------
void GdsMatrix::MakeRotation (float fAngle, float x, float y, float z)
{
	float sn, cs;
	sn = sin( fAngle );
	cs = cos( fAngle );

	//NiSinCos(fAngle, sn, cs);

	float omcs = 1.0f-cs;
	float x2 = x*x;
	float y2 = y*y;
	float z2 = z*z;
	float xym = x*y*omcs;
	float xzm = x*z*omcs;
	float yzm = y*z*omcs;
	float xsin = x*sn;
	float ysin = y*sn;
	float zsin = z*sn;

	m_pEntry[0][0] = x2*omcs+cs;
	m_pEntry[0][1] = xym+zsin;
	m_pEntry[0][2] = xzm-ysin;
	m_pEntry[1][0] = xym-zsin;
	m_pEntry[1][1] = y2*omcs+cs;
	m_pEntry[1][2] = yzm+xsin;
	m_pEntry[2][0] = xzm+ysin;
	m_pEntry[2][1] = yzm-xsin;
	m_pEntry[2][2] = z2*omcs+cs;
}
//---------------------------------------------------------------------------
bool GdsMatrix::operator== (const GdsMatrix& mat) const
{
	return
		(m_pEntry[0][0] == mat.m_pEntry[0][0]) &&
		(m_pEntry[0][1] == mat.m_pEntry[0][1]) &&
		(m_pEntry[0][2] == mat.m_pEntry[0][2]) &&
		(m_pEntry[1][0] == mat.m_pEntry[1][0]) &&
		(m_pEntry[1][1] == mat.m_pEntry[1][1]) &&
		(m_pEntry[1][2] == mat.m_pEntry[1][2]) &&
		(m_pEntry[2][0] == mat.m_pEntry[2][0]) &&
		(m_pEntry[2][1] == mat.m_pEntry[2][1]) &&
		(m_pEntry[2][2] == mat.m_pEntry[2][2]);
}
//---------------------------------------------------------------------------
GdsMatrix GdsMatrix::operator+ (const GdsMatrix& mat) const
{
	GdsMatrix result = *this;
	result.m_pEntry[0][0] += mat.m_pEntry[0][0];
	result.m_pEntry[0][1] += mat.m_pEntry[0][1];
	result.m_pEntry[0][2] += mat.m_pEntry[0][2];
	result.m_pEntry[1][0] += mat.m_pEntry[1][0];
	result.m_pEntry[1][1] += mat.m_pEntry[1][1];
	result.m_pEntry[1][2] += mat.m_pEntry[1][2];
	result.m_pEntry[2][0] += mat.m_pEntry[2][0];
	result.m_pEntry[2][1] += mat.m_pEntry[2][1];
	result.m_pEntry[2][2] += mat.m_pEntry[2][2];
	return result;
}
//---------------------------------------------------------------------------
GdsMatrix GdsMatrix::operator- (const GdsMatrix& mat) const
{
	GdsMatrix result = *this;
	result.m_pEntry[0][0] -= mat.m_pEntry[0][0];
	result.m_pEntry[0][1] -= mat.m_pEntry[0][1];
	result.m_pEntry[0][2] -= mat.m_pEntry[0][2];
	result.m_pEntry[1][0] -= mat.m_pEntry[1][0];
	result.m_pEntry[1][1] -= mat.m_pEntry[1][1];
	result.m_pEntry[1][2] -= mat.m_pEntry[1][2];
	result.m_pEntry[2][0] -= mat.m_pEntry[2][0];
	result.m_pEntry[2][1] -= mat.m_pEntry[2][1];
	result.m_pEntry[2][2] -= mat.m_pEntry[2][2];
	return result;
}
//---------------------------------------------------------------------------
GdsVector3 operator* (const GdsVector3& pt, const GdsMatrix& mat)
{
	return GdsVector3
		(
		pt.x*mat.m_pEntry[0][0]+pt.y*mat.m_pEntry[1][0]+
		pt.z*mat.m_pEntry[2][0],
		pt.x*mat.m_pEntry[0][1]+pt.y*mat.m_pEntry[1][1]+
		pt.z*mat.m_pEntry[2][1],
		pt.x*mat.m_pEntry[0][2]+pt.y*mat.m_pEntry[1][2]+
		pt.z*mat.m_pEntry[2][2]
	);
}
//---------------------------------------------------------------------------
bool GdsMatrix::Inverse (GdsMatrix& inv) const
{
	// Invert a 3x3 using cofactors.  This is about 8 times faster than
	// the Numerical Recipes code which uses Gaussian elimination.

	inv.m_pEntry[0][0] = m_pEntry[1][1]*m_pEntry[2][2]-
		m_pEntry[1][2]*m_pEntry[2][1];
	inv.m_pEntry[0][1] = m_pEntry[0][2]*m_pEntry[2][1]-
		m_pEntry[0][1]*m_pEntry[2][2];
	inv.m_pEntry[0][2] = m_pEntry[0][1]*m_pEntry[1][2]-
		m_pEntry[0][2]*m_pEntry[1][1];
	inv.m_pEntry[1][0] = m_pEntry[1][2]*m_pEntry[2][0]-
		m_pEntry[1][0]*m_pEntry[2][2];
	inv.m_pEntry[1][1] = m_pEntry[0][0]*m_pEntry[2][2]-
		m_pEntry[0][2]*m_pEntry[2][0];
	inv.m_pEntry[1][2] = m_pEntry[0][2]*m_pEntry[1][0]-
		m_pEntry[0][0]*m_pEntry[1][2];
	inv.m_pEntry[2][0] = m_pEntry[1][0]*m_pEntry[2][1]-
		m_pEntry[1][1]*m_pEntry[2][0];
	inv.m_pEntry[2][1] = m_pEntry[0][1]*m_pEntry[2][0]-
		m_pEntry[0][0]*m_pEntry[2][1];
	inv.m_pEntry[2][2] = m_pEntry[0][0]*m_pEntry[1][1]-
		m_pEntry[0][1]*m_pEntry[1][0];

	float fDet = m_pEntry[0][0]*inv.m_pEntry[0][0]+
		m_pEntry[0][1]*inv.m_pEntry[1][0]+
		m_pEntry[0][2]*inv.m_pEntry[2][0];
	if ( abs(fDet) <= 1e-06f )
		return false;

	float fInvdet = 1.0f/fDet;
	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			inv.m_pEntry[row][col] *= fInvdet;
		}
	}

	return true;
}
//---------------------------------------------------------------------------
GdsMatrix GdsMatrix::Inverse () const
{
	GdsMatrix inv;

	if ( Inverse(inv) == false )
		inv.MakeZero( );

	return inv;
}
//---------------------------------------------------------------------------
GdsMatrix GdsMatrix::Transpose () const
{
	GdsVector3 row[3];

	GetRow( 0, row[0] );
	GetRow( 1, row[1] );
	GetRow( 2, row[2] );

	return GdsMatrix( row[0], row[1], row[2] );
}