#include "GdsIndexBufferProperty.h"

GdsIndexBufferProperty::GdsIndexBufferProperty(void)
{
	SetName( OBJECT_PROPERTY_IB );
	SetType( INDEX_BUFFER );
	vClear();
}

GdsIndexBufferProperty::~GdsIndexBufferProperty(void)
{
	vClear();
}

void GdsIndexBufferProperty::vUpdate( float fElapsedTime )
{

}

void GdsIndexBufferProperty::vClear()
{

}