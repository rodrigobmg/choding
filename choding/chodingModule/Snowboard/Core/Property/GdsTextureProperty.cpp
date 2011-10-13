#include "GdsTextureProperty.h"

GdsTextureProperty::GdsTextureProperty(void):
m_eTexturingType( DEFAULT )
{
	SetName( OBJECT_PROPERTY_TEXTURE );
	SetType( TEXTURE );
	Clear();
	for ( size_t i = 0 ; i < MAX ; i++ )
		m_Tex.push_back( NULL );

}

GdsTextureProperty::~GdsTextureProperty(void)
{
	Clear();
}

void GdsTextureProperty::Clear()
{
	for_each( m_Tex.begin() , m_Tex.end() , functor::release() );
}

void GdsTextureProperty::Render( LPDIRECT3DDEVICE9 device )
{
	if ( m_eTexturingType == DEFAULT )
		device->SetTexture( 0, m_Tex[0] );  	
	else if ( m_eTexturingType == MULTI_TEXTURE )
	{
		
	}
}

void GdsTextureProperty::SetTexture( LPDIRECT3DTEXTURE9 texture , const int num /*= 0 */ )
{
	if ( ( m_eTexturingType == MULTI_TEXTURE ) && ( num != 0 ) )
		ASSERT( 0 );

	if ( m_Tex.size() < num )
		ASSERT( 0 );

	ASSERT( texture );

	if ( m_Tex[num] == NULL ) 
	{
		m_Tex[num] = texture; 
	}

	m_iCountOfTexture = m_Tex.size();
}

LPDIRECT3DTEXTURE9* GdsTextureProperty::GetTexture( const int num /*= 0 */ )
{
	if ( ( m_eTexturingType == MULTI_TEXTURE ) && ( num != 0 ) )
		ASSERT( 0 );
	if ( m_Tex.size() < num )
		ASSERT( 0 );
	return &(m_Tex[num]);
}

LPDIRECT3DTEXTURE9 GdsTextureProperty::GetTexturePtr( const int num /*= 0 */ )
{
	if ( ( m_eTexturingType == MULTI_TEXTURE ) && ( num != 0 ) )
		ASSERT( 0 );
	if ( m_Tex.size() < num )
		ASSERT( 0 );
	return m_Tex[num];
}
