#include "GdsPolygon.h"


GdsPolygon::GdsPolygon():
m_pVertexContainer(NULL),
m_iSizeVertex( 0 ),
m_iCountVertex( 0 ),
m_FVF( 0 )
{

}

GdsPolygon::~GdsPolygon()
{
	SAFE_DELETE( m_pVertexContainer );
}
