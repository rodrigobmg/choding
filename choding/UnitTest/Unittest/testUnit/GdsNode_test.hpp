
#pragma once
#include <boost\pool\singleton_pool.hpp>
#include "..\..\..\chodingModule\Snowboard\Core\SceneNode\GdsNode.h"


TEST( test ,  Module )
{
	for (size_t t = 0 ; t < 100 ; t++)
	{
		GdsNodePtr node = GdsNodePtr( new GdsNode );
	}
	
	GdsNodePtr node2 = GdsNodePtr( new GdsNode );
}
