#include "GdsMaterialProperty.h"
#include "..\..\System\Logger\logger.h"

GdsMaterialProperty::GdsMaterialProperty(void):
m_eTexturingType( DEFAULT )
{
	SetName( OBJECT_PROPERTY_TEXTURE );
	SetType( TEXTURE );
	Clear();
	for ( size_t i = 0 ; i < MAX ; i++ )
		m_Tex.push_back( NULL );

	ZeroMemory( &m_dxMaterial , sizeof(D3DMATERIAL9) );
	m_dxMaterial.Diffuse.r = m_dxMaterial.Ambient.r = 0.0f;
	m_dxMaterial.Diffuse.g = m_dxMaterial.Ambient.g = 0.0f;
	m_dxMaterial.Diffuse.b = m_dxMaterial.Ambient.b = 0.0f;
	m_dxMaterial.Diffuse.a = m_dxMaterial.Ambient.a = 1.0f;

}

GdsMaterialProperty::~GdsMaterialProperty(void)
{
	Clear();
}


void GdsMaterialProperty::SetAmbientColor( float r , float g , float b )
{
	m_dxMaterial.Ambient.r = r;
	m_dxMaterial.Ambient.g = g;
	m_dxMaterial.Ambient.b = b;
}

void GdsMaterialProperty::SetDiffuesColor( float r , float g , float b )
{
	m_dxMaterial.Diffuse.r = r;
	m_dxMaterial.Diffuse.g = g;
	m_dxMaterial.Diffuse.b = b;
}

void GdsMaterialProperty::SetSpecularColor( float r , float g , float b )
{
	m_dxMaterial.Specular.r = r;
	m_dxMaterial.Specular.g = g;
	m_dxMaterial.Specular.b = b;
}

void GdsMaterialProperty::Clear()
{
	for_each( m_Tex.begin() , m_Tex.end() , functor::release() );
}

void GdsMaterialProperty::Render( LPDIRECT3DDEVICE9 device )
{
	if ( m_eTexturingType == DEFAULT )
		device->SetTexture( 0, m_Tex[0] );  	
	else if ( m_eTexturingType == MULTI_TEXTURE )
	{
		LOG_WARNING( "미지원 기능" );
		ASSERT( 0 );
	}
}

void GdsMaterialProperty::SetTexture( LPDIRECT3DTEXTURE9 texture , const int num /*= 0 */ )
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

LPDIRECT3DTEXTURE9* GdsMaterialProperty::GetTexture( const int num /*= 0 */ )
{
	if ( ( m_eTexturingType == MULTI_TEXTURE ) && ( num != 0 ) )
		ASSERT( 0 );
	if ( m_Tex.size() < num )
		ASSERT( 0 );
	return &(m_Tex[num]);
}

LPDIRECT3DTEXTURE9 GdsMaterialProperty::GetTexturePtr( const int num /*= 0 */ )
{
	if ( ( m_eTexturingType == MULTI_TEXTURE ) && ( num != 0 ) )
		ASSERT( 0 );
	if ( m_Tex.size() < num )
		ASSERT( 0 );
	return m_Tex[num];
}

