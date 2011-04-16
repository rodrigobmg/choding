#include "GdsProperty.h"

GdsProperty::GdsProperty(void):
m_Animation( GdsAnimationPtr( (GdsAnimation*)NULL ) ),
m_Material( GdsMaterialPtr( (GdsMaterial*)NULL ) ),
m_Mesh( GdsMeshPtr( (GdsMesh*)NULL ) ),
m_Polygon( GdsPolygonPtr( (GdsPolygon*)NULL ) )
{
	SetName( OBJECT_PROPERTY );
	vClear();
}

GdsProperty::~GdsProperty(void)
{
	vClear();
}

GdsAnimationPtr GdsProperty::GetAnimation()
{
	if( m_Animation == NULL )
		m_Animation = GdsAnimationPtr( new GdsAnimation );

	return m_Animation;
}

GdsMaterialPtr GdsProperty::GetMaterial()
{
	if ( m_Material == NULL )
		m_Material = GdsMaterialPtr( new GdsMaterial );

	return m_Material;
}

GdsMeshPtr GdsProperty::GetMesh()
{
	if ( m_Mesh == NULL )
		m_Mesh = GdsMeshPtr( new GdsMesh );

	return m_Mesh;
}

GdsPolygonPtr GdsProperty::GetPolygon()
{
	if ( m_Polygon == NULL )
		m_Polygon = GdsPolygonPtr( new GdsPolygon );

	return m_Polygon;
}

void GdsProperty::vUpdate( float fElapsedTime )
{
}

void GdsProperty::vRender( LPDIRECT3DDEVICE9 device )
{
	if ( m_Mesh != NULL )
	{
		m_Mesh->Render(device);
	}
}