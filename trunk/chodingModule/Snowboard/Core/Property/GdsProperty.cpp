#include "GdsProperty.h"

GdsProperty::GdsProperty(void)
{
	SetName( OBJECT_PROPERTY );
	m_eType = NONE_TYPE;
	vClear();
}

GdsProperty::~GdsProperty(void)
{
	vClear();
}
