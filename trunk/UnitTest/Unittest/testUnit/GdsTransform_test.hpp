#pragma once

#include "..\..\..\chodingModule\Snowboard\Core\Transform\GdsTransform.h"
#include "..\..\..\chodingModule\Snowboard\Core\Transform\GdsVector.h"

TEST( GdsVector3Test  ,  Module )
{
	GdsVector3 pos( -1.f , -1.f , 10.f );
	GdsVector3 pos1( 1.f , -1.f , 0.f );

	GdsVector3 gdsv = pos1.Cross( pos );
	gdsv.Unitize();

	D3DXVECTOR3 dxv( -1.f , -1.f , 10.f );
	D3DXVECTOR3 dxv1( 1.f , -1.f , 0.f );
	D3DXVECTOR3 dxvoutput;
	D3DXVec3Cross( &dxvoutput , &dxv1 , &dxv );
	D3DXVec3Normalize( &dxvoutput , &dxvoutput );

	EXPECT_EQ( dxvoutput[0] , gdsv[0] );
	EXPECT_EQ( dxvoutput[1] , gdsv[1] );
	EXPECT_EQ( dxvoutput[2] , gdsv[2] );

	D3DXMATRIXA16 rotDX;
	GdsMatrix3 rotGDS;

	D3DXMatrixIdentity( &rotDX );
	rotGDS.MakeIdentity();
	
	rotGDS.MakeYRotation( 2.0f );
	D3DXMatrixRotationY( &rotDX , 2.0f );

	EXPECT_EQ( (float)rotGDS.m_pEntry[0][0] , (float)rotDX._11 );
	EXPECT_EQ( (float)rotGDS.m_pEntry[2][0] , (float)rotDX._31 );
	EXPECT_EQ( (float)rotGDS.m_pEntry[0][2] , (float)rotDX._13 );
	EXPECT_EQ( (float)rotGDS.m_pEntry[2][2] , (float)rotDX._33 );

}

