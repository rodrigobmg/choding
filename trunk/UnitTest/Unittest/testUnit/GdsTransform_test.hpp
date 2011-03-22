#pragma once

#include "..\..\..\chodingModule\Snowboard\Core\Transform\GdsTransform.h"
#include "..\..\..\chodingModule\Snowboard\Core\Transform\GdsVector.h"

TEST( GdsVector3Test  ,  Module )
{
	GdsVector3 pos( -1.f , -1.f , 10.f );
	GdsVector3 pos1( 1.f , -1.f , 0.f );

	EXPECT_EQ( 1 , pos1[0] );
	EXPECT_NE( pos , pos1 );

	GdsMatrix3 rot = GdsMatrix3::ZERO;
	GdsMatrix3 rot1 = GdsMatrix3::IDENTITY;
	EXPECT_NE( rot , rot1 );

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
}

