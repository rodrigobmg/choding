#ifndef _RENDERER_IRENDEROBJECT_H_
#define _RENDERER_IRENDEROBJECT_H_
#pragma once

#include "../../Common/Type/CommonType.h"
#include "../stdafx.h"


class IRenderObject
{
public:
	IRenderObject(void){};
	virtual ~IRenderObject(void){};

	enum ENABLE_RENDER_TYPE
	{			
		MESH = 0,
	};

	virtual void Draw( D3DDevice* pDevice , D3DXEffect* pEffect ) = 0;
	virtual void onDeviceReset( D3DDevice* pDevice ) = 0;
	virtual void onDeviceLost() = 0;
		
};

#endif _RENDERER_IRENDEROBJECT_H_