
#pragma once

#include "..\..\..\chodingModule\Snowboard\Core\Resource\Type\GdsVertexBuffer.h"
#include "..\..\..\chodingModule\Snowboard\Core\Resource\Type\GdsIndexBuffer.h"

TEST( GdsVertexBuffer , VertexBuffer )
{
	GdsVertexBufferPtr vertexbuffer = GdsVertexBufferPtr( new GdsVertexBuffer );
}

TEST( GdsIndexBuffer , IndexBuffer )
{
	GdsIndexBufferPtr indexbuffer = GdsIndexBufferPtr( new GdsIndexBuffer );
	indexbuffer->AddIndex( GDSINDEX( 0 , 1 , 2 ) );
	EXPECT_EQ( 1 , indexbuffer->GetIndexMaxCount() );
	EXPECT_EQ( false , indexbuffer->IsBufferEmpty() );
	indexbuffer->Alloc();
	EXPECT_NE( NULL , (BOOL)(indexbuffer->Get()) );
}