#pragma once

#include "..\..\..\chodingModule\Snowboard\Core\Property\GdsTextureProperty.h"
#include "..\..\..\chodingModule\Snowboard\Core\Property\GdsPropertyState.h"

TEST( GdsPropertyTest ,  Module )
{
	GdsPropertyStatePtr Property = GdsPropertyStatePtr( new GdsPropertyState );
	GdsTexturePropertyPtr Texture = Property->GetTextureProperty();
	LPDIRECT3DTEXTURE9 tex = Texture->GetTexturePtr();
	EXPECT_FALSE( tex );

	union MD2_VERTEX
	{
		struct
		{
			float x, y, z;
			float nx , ny , nz;
			float tu, tv;
		};
		struct
		{
			D3DXVECTOR3 v;
			D3DXVECTOR3 n; //노멀 벡터 추가
			D3DXVECTOR2 t;
		};
	};

	union MD2_VERTEX2
	{
		struct
		{
			float x, y, z;
			float nx , ny , nz;
			float tu, tv;
		};
		struct
		{
			D3DXVECTOR3 v;
			D3DXVECTOR3 n; //노멀 벡터 추가
			D3DXVECTOR2 t;
		};
	};

	void* vb = NULL;
	MD2_VERTEX st;
	vb = &st;
	//EXPECT_EQ( sizeof( st ) , ARRAYSIZE( vb ) );
}
