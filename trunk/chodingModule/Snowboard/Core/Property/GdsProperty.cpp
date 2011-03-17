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
		ASSERT( 0 && "�ѹ� Ÿ���� ���ϸ� �����Ҽ� ����." );
		return;
	}

	m_eType = type;
}