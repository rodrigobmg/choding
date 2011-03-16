
inline GdsMatrix3::GdsMatrix3()
{
	SetName( OBJECT_MATRIX3 );
}
//---------------------------------------------------------------------------
inline void GdsMatrix3::MakeZero ()
{
	SetCol(0, 0.0f, 0.0f, 0.0f);
	SetCol(1, 0.0f, 0.0f, 0.0f);
	SetCol(2, 0.0f, 0.0f, 0.0f);
}
//---------------------------------------------------------------------------
inline void GdsMatrix3::MakeRotation (float fAngle, const GdsVector3& axis)
{
	MakeRotation(fAngle,axis.x,axis.y,axis.z);
}
//---------------------------------------------------------------------------
//  Row access functions
inline void GdsMatrix3::GetRow( unsigned int uiRow, GdsVector3& row ) const
{
	assert( uiRow <= 2 );
	row.x   = m_pEntry[uiRow][0];
	row.y   = m_pEntry[uiRow][1];
	row.z   = m_pEntry[uiRow][2];
}
//---------------------------------------------------------------------------
inline void GdsMatrix3::SetRow( unsigned int uiRow, const GdsVector3& row )
{
	assert( uiRow <= 2 );
	m_pEntry[uiRow][0]  = row.x;
	m_pEntry[uiRow][1]  = row.y;
	m_pEntry[uiRow][2]  = row.z;
}
//---------------------------------------------------------------------------
inline void GdsMatrix3::GetRow( unsigned int uiRow, float& f0, float& f1, 
							  float& f2 ) const
{
	assert( uiRow <= 2 );
	f0 = m_pEntry[uiRow][0];
	f1 = m_pEntry[uiRow][1];
	f2 = m_pEntry[uiRow][2];
}
//---------------------------------------------------------------------------
inline void GdsMatrix3::SetRow( unsigned int uiRow, float f0, float f1, 
							  float f2 )
{
	assert( uiRow <= 2 );
	m_pEntry[uiRow][0]  = f0;
	m_pEntry[uiRow][1]  = f1;
	m_pEntry[uiRow][2]  = f2;
}
//---------------------------------------------------------------------------
inline void GdsMatrix3::GetRow( unsigned int uiRow, float* pRow ) const
{
	assert( uiRow <= 2 );
	pRow[0] = m_pEntry[uiRow][0];
	pRow[1] = m_pEntry[uiRow][1];
	pRow[2] = m_pEntry[uiRow][2];
}
//---------------------------------------------------------------------------
inline void GdsMatrix3::SetRow( unsigned int uiRow, const float* pRow )
{
	assert( uiRow <= 2 );
	m_pEntry[uiRow][0]  = pRow[0];
	m_pEntry[uiRow][1]  = pRow[1];
	m_pEntry[uiRow][2]  = pRow[2];
}

//---------------------------------------------------------------------------
//  Column access functions
inline void GdsMatrix3::GetCol( unsigned int uiCol, GdsVector3& col ) const
{
	assert( uiCol <= 2 );
	col.x   = m_pEntry[0][uiCol];
	col.y   = m_pEntry[1][uiCol];
	col.z   = m_pEntry[2][uiCol];
}
//---------------------------------------------------------------------------
inline void GdsMatrix3::SetCol( unsigned int uiCol, const GdsVector3& col )
{
	assert( uiCol <= 2 );
	m_pEntry[0][uiCol]  = col.x;
	m_pEntry[1][uiCol]  = col.y;
	m_pEntry[2][uiCol]  = col.z;
}
//---------------------------------------------------------------------------
inline void GdsMatrix3::GetCol( unsigned int uiCol, float& f0, float& f1, 
							  float& f2 ) const
{
	assert( uiCol <= 2 );
	f0 = m_pEntry[0][uiCol];
	f1 = m_pEntry[1][uiCol];
	f2 = m_pEntry[2][uiCol];
}
//---------------------------------------------------------------------------
inline void GdsMatrix3::SetCol( unsigned int uiCol, float f0, float f1, 
							  float f2 )
{
	assert( uiCol <= 2 );
	m_pEntry[0][uiCol] = f0;
	m_pEntry[1][uiCol] = f1;
	m_pEntry[2][uiCol] = f2;
}
//---------------------------------------------------------------------------
inline void GdsMatrix3::GetCol( unsigned int uiCol, float* pCol ) const
{
	assert( uiCol <= 2 );
	pCol[0] = m_pEntry[0][uiCol];
	pCol[1] = m_pEntry[1][uiCol];
	pCol[2] = m_pEntry[2][uiCol];
}
//---------------------------------------------------------------------------
inline void GdsMatrix3::SetCol( unsigned int uiCol, const float* pCol )
{
	assert( uiCol <= 2 );
	m_pEntry[0][uiCol] = pCol[0];
	m_pEntry[1][uiCol] = pCol[1];
	m_pEntry[2][uiCol] = pCol[2];
}

//---------------------------------------------------------------------------
//  Entry access functions
inline float GdsMatrix3::GetEntry( unsigned int uiRow, 
								 unsigned int uiCol ) const
{
	assert( uiRow <= 2 && uiCol <= 2 );
	return m_pEntry[uiRow][uiCol];
}
//---------------------------------------------------------------------------
inline void GdsMatrix3::SetEntry( unsigned int uiRow, unsigned int uiCol, 
								float fEntry )
{
	assert( uiRow <= 2 && uiCol <= 2 );
	m_pEntry[uiRow][uiCol]  = fEntry;
}

//---------------------------------------------------------------------------
inline GdsMatrix3 GdsMatrix3::operator* (const GdsMatrix3& mat) const
{
	GdsMatrix3 prd;

	prd.m_pEntry[0][0] =
		m_pEntry[0][0]*mat.m_pEntry[0][0]+
		m_pEntry[0][1]*mat.m_pEntry[1][0]+
		m_pEntry[0][2]*mat.m_pEntry[2][0];
	prd.m_pEntry[1][0] =
		m_pEntry[1][0]*mat.m_pEntry[0][0]+
		m_pEntry[1][1]*mat.m_pEntry[1][0]+
		m_pEntry[1][2]*mat.m_pEntry[2][0];
	prd.m_pEntry[2][0] =
		m_pEntry[2][0]*mat.m_pEntry[0][0]+
		m_pEntry[2][1]*mat.m_pEntry[1][0]+
		m_pEntry[2][2]*mat.m_pEntry[2][0];
	prd.m_pEntry[0][1] =
		m_pEntry[0][0]*mat.m_pEntry[0][1]+
		m_pEntry[0][1]*mat.m_pEntry[1][1]+
		m_pEntry[0][2]*mat.m_pEntry[2][1];
	prd.m_pEntry[1][1] =
		m_pEntry[1][0]*mat.m_pEntry[0][1]+
		m_pEntry[1][1]*mat.m_pEntry[1][1]+
		m_pEntry[1][2]*mat.m_pEntry[2][1];
	prd.m_pEntry[2][1] =
		m_pEntry[2][0]*mat.m_pEntry[0][1]+
		m_pEntry[2][1]*mat.m_pEntry[1][1]+
		m_pEntry[2][2]*mat.m_pEntry[2][1];
	prd.m_pEntry[0][2] =
		m_pEntry[0][0]*mat.m_pEntry[0][2]+
		m_pEntry[0][1]*mat.m_pEntry[1][2]+
		m_pEntry[0][2]*mat.m_pEntry[2][2];
	prd.m_pEntry[1][2] =
		m_pEntry[1][0]*mat.m_pEntry[0][2]+
		m_pEntry[1][1]*mat.m_pEntry[1][2]+
		m_pEntry[1][2]*mat.m_pEntry[2][2];
	prd.m_pEntry[2][2] =
		m_pEntry[2][0]*mat.m_pEntry[0][2]+
		m_pEntry[2][1]*mat.m_pEntry[1][2]+
		m_pEntry[2][2]*mat.m_pEntry[2][2];

	return prd;
}
//---------------------------------------------------------------------------
inline GdsMatrix3 GdsMatrix3::operator* (float fScalar) const
{
	GdsMatrix3 result;
	result.m_pEntry[0][0] = m_pEntry[0][0] * fScalar;
	result.m_pEntry[0][1] = m_pEntry[0][1] * fScalar;
	result.m_pEntry[0][2] = m_pEntry[0][2] * fScalar;
	result.m_pEntry[1][0] = m_pEntry[1][0] * fScalar;
	result.m_pEntry[1][1] = m_pEntry[1][1] * fScalar;
	result.m_pEntry[1][2] = m_pEntry[1][2] * fScalar;
	result.m_pEntry[2][0] = m_pEntry[2][0] * fScalar;
	result.m_pEntry[2][1] = m_pEntry[2][1] * fScalar;
	result.m_pEntry[2][2] = m_pEntry[2][2] * fScalar;
	return result;
}
//---------------------------------------------------------------------------
inline GdsVector3 GdsMatrix3::operator* (const GdsVector3& pt) const
{
	return GdsVector3
		(
		m_pEntry[0][0]*pt.x+m_pEntry[0][1]*pt.y+m_pEntry[0][2]*pt.z,
		m_pEntry[1][0]*pt.x+m_pEntry[1][1]*pt.y+m_pEntry[1][2]*pt.z,
		m_pEntry[2][0]*pt.x+m_pEntry[2][1]*pt.y+m_pEntry[2][2]*pt.z
		);
}
//---------------------------------------------------------------------------
inline bool GdsMatrix3::operator!=(const GdsMatrix3& mat) const
{
	return !(*this == mat);
}
//---------------------------------------------------------------------------
