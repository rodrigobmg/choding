#pragma once

#include "..\..\..\chodingModule\Snowboard\Core\Transform\GdsTransform.h"
#include "..\..\..\chodingModule\Snowboard\Core\Transform\GdsVector.h"
#include "..\..\..\chodingModule\Snowboard\Core\Transform\GdsMatrix.h"

TEST( GdsTransform  ,  Module )
{
	GdsVector3 pos = GdsVector3::ZERO;
	GdsVector3 pos1( 1.f , -1.f , 0.f );
	float f = pos1.Length();
	GdsVector3 possum;
	EXPECT_EQ( 10 , pos.x );
	EXPECT_EQ( -10 , pos.y );
	EXPECT_EQ( 0 , pos.z );

	

	//pos = pos1 * 10;
	//GdsVector3 posdivided = possum / 2;

	GdsMatrix mat;

	GdsTransform world;
	world.MakeIdentity();
	GdsTransform local;
	local.MakeIdentity();
}

