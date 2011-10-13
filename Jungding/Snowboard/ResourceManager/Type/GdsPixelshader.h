#ifndef _GDS_CORE_RESOURCE_PIXELSHADER_H_
#define _GDS_CORE_RESOURCE_PIXELSHADER_H_


class GdsPixelShader : public GdsResBase
{
private:

	protected:
	
	virtual void				vClear();
	virtual HRESULT				vRelease();
	virtual HRESULT				vLoadResource(LPDIRECT3DDEVICE9 device);

public:
	GdsPixelShader();
	virtual ~GdsPixelShader();	

};

typedef boost::shared_ptr< GdsPixelShader >	GdsPixelShaderPtr;

#endif
