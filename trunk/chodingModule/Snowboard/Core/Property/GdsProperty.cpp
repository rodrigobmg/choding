#include "GdsProperty.h"

GdsProperty::GdsProperty(void)
{
	SetName( OBJECT_PROPERTY );
	vClear();
}

GdsProperty::~GdsProperty(void)
{
	vClear();
}
