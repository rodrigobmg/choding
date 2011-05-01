#include "GdsOctree.h"

GdsOctree::GdsOctree( D3DXVECTOR3* pBuffer , size_t buffer_size , int iMaxDepth ):
m_minPos( 0, 0, 0 ),
m_maxPos( 0, 0, 0 )
{
	
	for ( size_t i = 0; i < buffer_size ; ++i )
	{
		if ( m_minPos.x > pBuffer[i].x )
			m_minPos.x = pBuffer[i].x;

		if ( m_minPos.y > pBuffer[i].y )
			m_minPos.y = pBuffer[i].y;

		if ( m_minPos.z > pBuffer[i].z )
			m_minPos.z = pBuffer[i].z;

		if ( m_maxPos.x < pBuffer[i].x )
			m_maxPos.x = pBuffer[i].x;

		if ( m_maxPos.y < pBuffer[i].y )
			m_maxPos.y = pBuffer[i].y;

		if ( m_maxPos.z < pBuffer[i].z )
			m_maxPos.z = pBuffer[i].z;
	}
}

GdsOctree::~GdsOctree()
{

}

void GdsOctree::Build()
{
	devide( 0 );	
}

bool GdsOctree::devide( int iCurDepth )
{
	iCurDepth = iCurDepth;
	return iCurDepth;
}