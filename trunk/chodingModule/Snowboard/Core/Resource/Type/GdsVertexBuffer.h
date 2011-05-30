#ifndef _GDS_CORE_RESOURCE_VERTEXBUFFER_H_
#define _GDS_CORE_RESOURCE_VERTEXBUFFER_H_

#include "../GdsResBaseType.h"

class GdsVertexBuffer : public GdsResBase
{
private:

	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	std::vector<GDSVERTEX>		m_pBuffer;

	int							m_VertexSize;
	int							m_FVF;
	int							m_Vertex_Maxcount;
	int							m_iStartVertexIndex;
	int							m_iEndVertexIndex;

protected:
	
	virtual void				vClear();
	virtual HRESULT				vRelease();
	virtual HRESULT				vLoadResource(LPDIRECT3DDEVICE9 device);

public:
	GdsVertexBuffer();
	virtual ~GdsVertexBuffer();	

	LPDIRECT3DVERTEXBUFFER9		Get(){ return m_pVB; }
	
	void						GetVertexFromIndex( int index , GDSVERTEX& vertex );
	void						SetVertexToIndex( int index , GDSVERTEX& vertex );
	void						AddVertex( GDSVERTEX& vertex );
	void						Alloc();
	void						Free();
	void						Update();

	void						SetVertexSize( int ivertexsize ){ m_VertexSize = ivertexsize; }
	void						SetFVF( int ifvf ){ m_FVF = ifvf; }
	void						SetVertexMaxCount( int icount ){ m_Vertex_Maxcount = icount; }
	int							GetVertexMaxCount(){ return m_Vertex_Maxcount; }
	void						SetStartVertexIndex( int index ){ m_iStartVertexIndex = index; }
	void						SetEndVertexIndex( int index ){ m_iEndVertexIndex = index; }
};

typedef boost::shared_ptr< GdsVertexBuffer >	GdsVertexBufferPtr;

#endif
