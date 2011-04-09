#include "GdsMaterialProperty.h"

GdsMaterialProperty::GdsMaterialProperty()
{
	ZeroMemory( &m_dxMaterial , sizeof(D3DMATERIAL9) );
	m_dxMaterial.Diffuse.r = m_dxMaterial.Ambient.r = 0.0f;
	m_dxMaterial.Diffuse.g = m_dxMaterial.Ambient.g = 0.0f;
	m_dxMaterial.Diffuse.b = m_dxMaterial.Ambient.b = 0.0f;
	m_dxMaterial.Diffuse.a = m_dxMaterial.Ambient.a = 1.0f;
}

GdsMaterialProperty::~GdsMaterialProperty()
{

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

