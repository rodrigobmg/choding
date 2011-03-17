#include "GdsTextureProperty.h"

GdsTextureProperty::GdsTextureProperty(void)
{
	SetName( OBJECT_PROPERTY_TEXTURE );
	SetType( TEXTURE );
	vClear();
}

GdsTextureProperty::~GdsTextureProperty(void)
{
	vClear();
}

void GdsTextureProperty::vUpdate( float fElapsedTime )
{

}

void GdsTextureProperty::vClear()
{

}