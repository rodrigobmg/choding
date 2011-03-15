#pragma once

#include "..\..\..\chodingModule\Snowboard\Core\Transform\GdsTransform.h"
#include "..\..\..\chodingModule\Snowboard\Core\Transform\GdsVector.h"

TEST( GdsVector3Test  ,  Module )
{
	GdsVector3 pos = GdsVector3::ZERO;
	GdsVector3 pos1( 1.f , -1.f , 0.f );

	EXPECT_EQ( 1 , pos1[0] );
	EXPECT_NE( pos , pos1 );

	GdsMatrix3 rot = GdsMatrix3::ZERO;
	GdsMatrix3 rot1 = GdsMatrix3::IDENTITY;
	EXPECT_NE( rot , rot1 );
}

