#pragma once 

#include "Snowboard_type.h"

#include <Windows.h>
#include <d3dx9.h>
#include <assert.h>
#include <d3d9.h>
#include <tchar.h>
#include <String>
#include <Strsafe.h>

#include "../Utility/Utility.h"

#define CORE_RENDERER		L"RendererCore"
#define CORE_CAMERA			L"CameraCore"
#define CORE_RESOURCE		L"ResourceCore"

#define OBJECT_BASE			L"Base"
#define OBJECT_BASE_CORE	L"CoreBase"
#define OBJECT_RENDERBASE	L"RenderBase"
#define OBJECT_RENDERERDX9	L"RendererDX9"
#define OBJECT_COREMGR		L"CoreMgr"
#define OBJECT_RES_BASE		L"ResBase"
#define OBJECT_RES_TEXTURE	L"ResTypeTexture"
#define OBJECT_RES_MGR		L"ResMgr"



#define _LOGGER_ON_

#define SAFE_DELETE(v) {if(v){delete v; v = NULL;}}
#define SAFE_DELETE_ARRAY(v) { if(v) { delete[] v; v = NULL; } }
#define SAFE_RELEASE(v) {if(v){v->Release(); } }

#define ASSERT( exp )	assert( exp )

#define ADD_FLAGS( var, flags )		( (var) |=  (flags) )
#define REM_FLAGS( var, flags )		( (var) &= ~(flags) )
#define CHK_FLAGS( var, flags )		( (var) &   (flags) )

using namespace std;
typedef basic_string<TCHAR> tstring;