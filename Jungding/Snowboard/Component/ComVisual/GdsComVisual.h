#ifndef _SNOWBOARD_COMPONENT_VISUAL_H_
#define _SNOWBOARD_COMPONENT_VISUAL_H_

class GdsIndexBuffer;
class GdsVertexBuffer;
class GdsVertexShader;
class GdsPixelShader;

class GdsComVisual : public GdsComponent
{
public:
	GdsComVisual();
	virtual ~GdsComVisual();

	virtual const goc_id_type familyID() const	{ return COMPONENT_VISUAL_FAMILY; }

	virtual	void	Render( float fAccumtime ) = 0;

protected:

	GdsIndexBuffer*			m_pIndexbuffer;
	GdsVertexBuffer*		m_pVertexbuffer;
	GdsVertexShader*		m_pShadervertex;
	GdsPixelShader*			m_pShaderpixel;

};
#endif