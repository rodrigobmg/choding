#include "GdsProperty.h"
#include "GdsTextureProperty.h"
#include "GdsVertexBufferProperty.h"
#include "GdsIndexBufferProperty.h"

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

	case VERTEX_BUFFER:
		return GdsVertexBufferPropertyPtr( new GdsVertexBufferProperty );
		break;
		
	case INDEX_BUFFER:
		return GdsIndexBufferPropertyPtr( new GdsIndexBufferProperty );
		break;
	}
	ASSERT( 0 && "없는 타입");
}