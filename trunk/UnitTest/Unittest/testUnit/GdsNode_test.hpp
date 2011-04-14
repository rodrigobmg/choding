
#pragma once
#include <boost\pool\singleton_pool.hpp>
#include "..\..\..\chodingModule\Snowboard\Core\SceneNode\GdsNode.h"


TEST( GdsNodeTest ,  Module )
{
	GdsNodePtr parent = GdsNodePtr( new GdsNode );	
	GdsNodePtr child = GdsNodePtr( new GdsNode );

	parent->AttachChild( child );
	parent->Update( 0.0f );
}

