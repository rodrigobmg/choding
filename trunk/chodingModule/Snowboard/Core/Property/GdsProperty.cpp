#include "GdsProperty.h"

GdsProperty::GdsProperty(void)
{
	SetName( OBJECT_PROPERTY );
	m_eType = NONE_TYPE;
}

GdsProperty::~GdsProperty(void)
{
}

void GdsProperty::Update( float fElapsedTime )
{
	vUpdate( fElapsedTime );
}

void GdsProperty::SetType( PROPERTY_TYPE type )
{
	if ( m_eType != NONE_TYPE )
	{
		ASSERT( 0 && "한번 타입을 정하면 변경할수 없다." );
		return;
	}

	m_eType = type;
}