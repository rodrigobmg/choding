#include "stdafx.h"

GdsComVisual::GdsComVisual()
:m_pVertexbuffer( (GdsVertexBuffer*)NULL )
,m_pIndexbuffer( (GdsIndexBuffer*)NULL )
,m_pShadervertex( (GdsVertexShader*)NULL )
,m_pShaderpixel( (GdsPixelShader*)NULL )
{
}

GdsComVisual::~GdsComVisual()
{

}