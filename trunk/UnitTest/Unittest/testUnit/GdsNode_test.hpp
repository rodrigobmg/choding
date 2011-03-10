
#pragma once

#include "..\..\..\chodingModule\Snowboard\Core\SceneNode\GdsMeshNode.h"
#include "..\..\..\chodingModule\Snowboard\Core\SceneNode\GdsNode.h"

TEST( test ,  Module )
{
	GdsNodePtr rootnode = GdsNodePtr( new GdsNode );
	
	GdsMeshNodePtr meshnode = GdsMeshNodePtr( new GdsMeshNode );
	GdsMeshNodePtr meshnode_child = GdsMeshNodePtr( new GdsMeshNode );
	GdsMeshNodePtr meshnode_child2 = GdsMeshNodePtr( new GdsMeshNode );

	EXPECT_EQ( S_OK , rootnode->AttachChild( meshnode ) );
	
	EXPECT_EQ( S_OK , meshnode_child->AttachChild( meshnode_child2 ) );

	GdsMeshNodePtr child = boost::shared_dynamic_cast< GdsMeshNode >( rootnode->GetAt(0) );	
	EXPECT_EQ( S_OK , child->AttachChild( meshnode_child ) );
	EXPECT_EQ( TRUE , rootnode->Update( 0.f ) );
	
}
