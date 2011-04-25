#include "GdsMaterial.h"
#include "..\..\..\System\Logger\logger.h"

GdsMaterial::GdsMaterial(void):
m_eTexturingType( DEFAULT )
{
	SetName( OBJECT_PROPERTY_TEXTURE );
	vClear();
	for ( size_t i = 0 ; i < MAX ; i++ )
	{
		m_Tex.push_back( GdsResTexturePtr( (GdsResTexture*)NULL ) );
		m_TexturePath.push_back( L"" );
	}

	ZeroMemory( &m_dxMaterial , sizeof(D3DMATERIAL9) );
	m_dxMaterial.Diffuse.r = m_dxMaterial.Ambient.r = 1.0f;
	m_dxMaterial.Diffuse.g = m_dxMaterial.Ambient.g = 1.0f;
	m_dxMaterial.Diffuse.b = m_dxMaterial.Ambient.b = 1.0f;
	m_dxMaterial.Diffuse.a = m_dxMaterial.Ambient.a = 1.0f;

}

GdsMaterial::~GdsMaterial(void)
{
	vClear();
}


void GdsMaterial::SetAmbientColor( float r , float g , float b )
{
	m_dxMaterial.Ambient.r = r;
	m_dxMaterial.Ambient.g = g;
	m_dxMaterial.Ambient.b = b;
}

void GdsMaterial::SetDiffuesColor( float r , float g , float b )
{
	m_dxMaterial.Diffuse.r = r;
	m_dxMaterial.Diffuse.g = g;
	m_dxMaterial.Diffuse.b = b;
}

void GdsMaterial::SetSpecularColor( float r , float g , float b )
{
	m_dxMaterial.Specular.r = r;
	m_dxMaterial.Specular.g = g;
	m_dxMaterial.Specular.b = b;
}

void GdsMaterial::vClear()
{
	m_Tex.clear();
	m_SubMaterial.clear();
}

void GdsMaterial::vRender( LPDIRECT3DDEVICE9 device )
{
 	if ( m_eTexturingType == DEFAULT )
 		device->SetTexture( 0, m_Tex[0]->Get() );  	
// 	else if ( m_eTexturingType == MULTI_TEXTURE )
// 	{
// 		LOG_WARNING( "미지원 기능" );
// 		ASSERT( 0 );
// 	}
}

void GdsMaterial::SetTexture( GdsResTexturePtr texture , const int num /*= 0 */ )
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

GdsResTexturePtr GdsMaterial::GetTexture( const int num /*= 0 */ )
{
	if ( ( m_eTexturingType == MULTI_TEXTURE ) && ( num != 0 ) )
		ASSERT( 0 );
	if ( m_Tex.size() < num )
		ASSERT( 0 );
	return m_Tex[num];
}
