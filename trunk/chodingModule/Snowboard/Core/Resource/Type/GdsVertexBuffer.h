#ifndef _GDS_CORE_RESOURCE_VERTEXBUFFER_H_
#define _GDS_CORE_RESOURCE_VERTEXBUFFER_H_

#include "../GdsResBaseType.h"

class GdsVertexBuffer : public GdsResBase
{
private:

	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	GDSVERTEX*					m_pBuffer;

protected:
	
	virtual void				vClear();
	virtual HRESULT				vRelease();
	virtual HRESULT				vLoadResource(LPDIRECT3DDEVICE9 device);

public:
	GdsVertexBuffer();
	virtual ~GdsVertexBuffer();	

	LPDIRECT3DVERTEXBUFFER9		Get(){ return m_pVB; }
};

typedef boost::shared_ptr< GdsVertexBuffer >	GdsVertexBufferPtr;

#endif
