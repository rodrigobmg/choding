#include "GdsVertexBufferProperty.h"

GdsVertexBufferProperty::GdsVertexBufferProperty(void)
{
	SetName( OBJECT_PROPERTY_VB );
	SetType( VERTEX_BUFFER );
	vClear();
}

GdsVertexBufferProperty::~GdsVertexBufferProperty(void)
{
	vClear();
}

void GdsVertexBufferProperty::vUpdate( float fElapsedTime )
{

}

void GdsVertexBufferProperty::vClear()
{

}