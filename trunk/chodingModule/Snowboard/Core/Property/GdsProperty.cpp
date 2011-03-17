#include "GdsProperty.h"
#include "GdsTextureProperty.h"
#include "GdsPolygonProperty.h"

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


void GdsProperty::SetType( PROPERTY_TYPE type )
{
	if ( m_eType != NONE_TYPE )
	{
		ASSERT( 0 && "�ѹ� Ÿ���� ���ϸ� �����Ҽ� ����." );
		return;
	}

	m_eType = type;
}

void GdsProperty::vClear()
{
	m_PropertyContainer.clear();
}

GdsPropertyPtr GdsProperty::GetProperty( PROPERTY_TYPE type )
{
 	PROPERTY_MAP::iterator it;
 	it = m_PropertyContainer.find( type );
 	if ( it == m_PropertyContainer.end() )
 	{
 		GdsPropertyPtr pNewProperty = create_property( type );
		m_PropertyContainer.insert( std::pair< PROPERTY_TYPE , GdsPropertyPtr>( type ,pNewProperty ) );
		return pNewProperty;
 	}
	return it->second;
}

GdsPropertyPtr GdsProperty::create_property( PROPERTY_TYPE type )
{
	switch ( type )
	{
	case TEXTURE:
		return GdsTexturePropertyPtr( new GdsTextureProperty );
		break;

	case POLYGON:
		return GdsPolygonPropertyPtr( new GdsPolygonProperty );
		break;
		
	}
	ASSERT( 0 && "���� Ÿ��");
	return GdsPropertyPtr( (GdsProperty*)NULL );
}

void GdsProperty::vUpdate( float fElapsedTime )
{
	ASSERT( 0 && "�θ�ü�� ������Ʈ �Ǿ �ȵȴ�." );
}