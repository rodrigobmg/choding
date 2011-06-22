
#pragma once
#include <boost\pool\singleton_pool.hpp>
#include "..\..\..\chodingModule\Snowboard\Core\SceneNode\GdsNode.h"


TEST( GdsNodeTest ,  Module )
{
	GdsNodePtr parent = GdsNodePtr( new GdsNode );	
	GdsNodePtr child = GdsNodePtr( new GdsNode );
	typedef struct {
		int a;
		byte b;
		int c;
		byte d;
	}test1;

	int sizea = sizeof( test1 );

	typedef struct {		
		byte b;		
		byte d;
		int a;		
		int c;		
	}test2;

	int sizeb = sizeof( test2 );
}

