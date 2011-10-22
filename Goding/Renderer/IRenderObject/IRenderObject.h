#ifndef _RENDERER_IRENDEROBJECT_H_
#define _RENDERER_IRENDEROBJECT_H_
#pragma once

#include "../../Common/Type/type.h"
#include "../stdafx.h"


class IRenderObject
{
public:
	IRenderObject(void);
	virtual ~IRenderObject(void);

	enum ENABLE_RENDER_TYPE
	{
		STATIC_MESH,
		SKINNED_MESH,
		MORPHING_MESH,
	};

	virtual void Draw( D3DDevice* pDevice , D3DXEffect* pEffect ) = 0;
	virtual void Update( float fAccumtime ) = 0;
	virtual void onDeviceReset() = 0;
	virtual void onDeviceLost() = 0;
};

#endif _RENDERER_IRENDEROBJECT_H_