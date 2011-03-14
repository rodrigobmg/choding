#ifndef _GDS_CORE_TRANSFORM_MATRIX_H_
#define _GDS_CORE_TRANSFORM_MATRIX_H_

#include "../Base/GdsObject.h"
#include "GdsVector.h"

class GdsMatrix : public GdsObject
{
public:

	float m_pEntry[3][3];

	static const GdsMatrix ZERO;
	static const GdsMatrix IDENTITY;

	inline GdsMatrix();
	GdsMatrix(const GdsVector3& col0, const GdsVector3& col1, 
		const GdsVector3& col2);

	//  Row access functions
	inline void GetRow(unsigned int uiRow, GdsVector3& row) const;
	inline void SetRow(unsigned int uiRow, const GdsVector3& row);
	inline void GetRow(unsigned int uiRow,
		float& f0, float& f1, float& f2) const;
	inline void SetRow(unsigned int uiRow, float f0, float f1, float f2);
	inline void GetRow(unsigned int uiRow, float* pRow) const;
	inline void SetRow(unsigned int uiRow, const float* pRow);

	//  Column access functions
	inline void GetCol(unsigned int uiCol, GdsVector3& col) const;
	inline void SetCol(unsigned int uiCol, const GdsVector3& col);
	inline void GetCol(unsigned int uiCol,
		float& f0, float& f1, float& f2) const;
	inline void SetCol(unsigned int uiCol, float f0, float f1, float f2);
	inline void GetCol(unsigned int uiCol, float* pCol) const;
	inline void SetCol(unsigned int uiCol, const float* pCol);

	//  Entry access functions
	inline float GetEntry(unsigned int uiRow, unsigned int uiCol) const;
	inline void SetEntry(unsigned int uiRow, unsigned int uiCol, float fEntry);

	inline void MakeZero();
	void MakeIdentity();
	void MakeDiagonal(float fDiag0, float fDiag1, float fDiag2);
	void MakeXRotation(float fAngle);
	void MakeYRotation(float fAngle);
	void MakeZRotation(float fAngle);
	void MakeRotation(float fAngle, float x, float y, float z);
	inline void MakeRotation(float fAngle, const GdsVector3& axis);

	bool operator==(const GdsMatrix& mat) const;
	inline bool operator!=(const GdsMatrix& mat) const;

	GdsMatrix operator+(const GdsMatrix& mat) const;
	GdsMatrix operator-(const GdsMatrix& mat) const;
	inline GdsMatrix operator*(const GdsMatrix& mat) const;

	inline GdsMatrix operator*(float fScalar) const;

	// use this operator to compute MATRIX*VECTOR
	inline GdsVector3 operator*(const GdsVector3& pt) const;

	GdsMatrix Inverse() const;
	bool Inverse(GdsMatrix& inv) const;
	GdsMatrix Transpose() const;
};

#include "GdsMatrix.inl"

#endif