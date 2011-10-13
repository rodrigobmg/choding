#include "stdafx.h"


GdsComVisualRect::GdsComVisualRect()
{

}

GdsComVisualRect::~GdsComVisualRect()
{
	
}

void GdsComVisualRect::Make()
{

}

void GdsComVisualRect::Render( float fAccumtime )
{
	GdsGameObject* pObject = GetOwnerGO();
	if ( pObject == NULL )
		return;

	RENDERER.DrawBox( pObject->GetMatrix() , D3DXVECTOR3( 0,0,0 ) , D3DXVECTOR3( 10,10,10) );
}