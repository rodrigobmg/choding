#ifndef _GDS_CORE_RESOURCE_VERTEXSHADER_H_
#define _GDS_CORE_RESOURCE_VERTEXSHADER_H_

#include "../GdsResBaseType.h"

class GdsVertexShader : public GdsResBase
{
private:

protected:
	
	virtual void				vClear();
	virtual HRESULT				vRelease();
	virtual HRESULT				vLoadResource(LPDIRECT3DDEVICE9 device);

public:
	GdsVertexShader();
	virtual ~GdsVertexShader();	

};

typedef boost::shared_ptr< GdsVertexShader >	GdsVertexShaderPtr;

#endif
