#include "GdsBaseProperty.h"

GdsBaseProperty::GdsBaseProperty(void)
{
	SetName( OBJECT_PROPERTY );
	vClear();
}

GdsBaseProperty::~GdsBaseProperty(void)
{
	vClear();
}
