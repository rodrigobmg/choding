#ifndef _GDS_CORE_PROPERTY_POLYGON_H_
#define _GDS_CORE_PROPERTY_POLYGON_H_

#include "../Base/GdsBaseProperty.h"

class GdsPolygon : public GdsBaseProperty
{

	int				m_iCountVertex;
	int				m_iSizeVertex;
	void*			m_pVertexContainer;
	int				m_FVF;

public:
	GdsPolygon();
	virtual ~GdsPolygon();

	void			SetCountVertex( int icount ){ m_iCountVertex = icount; }
	void			SetFVF( int fvf ){ m_FVF = fvf; }
	void			SetSizeofVertex( int isize ){ m_iSizeVertex = isize; }

	int				GetCountVertex(){ return m_iCountVertex; }
	int				GetFVF(){ return m_FVF; }
	int				GetSizeofVertex(){ return m_iSizeVertex; }

	void			SetVertexContainer( void* pv ){ m_pVertexContainer = pv; }
	void*			GetVertexContainer(){ return m_pVertexContainer; }

};

typedef boost::shared_ptr< GdsPolygon > GdsPolygonPtr;

#endif