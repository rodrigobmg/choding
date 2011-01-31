#pragma once 

#include "Snowboard_type.h"

#include <Windows.h>
#include <d3dx9.h>
#include <assert.h>
#include <d3d9.h>
#include <tchar.h>
#include <HASH_MAP>
#include <String>
#include <Strsafe.h>


#define CORE_RENDERER		L"RendererCore"
#define CORE_CAMERA			L"CameraCore"
#define CORE_RESOURCE		L"ResourceCore"

#define OBJECT_BASE			L"Base"
#define OBJECT_RENDERBASE	L"RenderBase"
#define OBJECT_RENDERERDX9	L"RendererDX9"
#define OBJECT_COREMGR		L"CoreMgr"
#define OBJECT_RES_BASE		L"ResBase"
#define OBJECT_RES_TEXTURE	L"ResTypeTexture"
#define OBJECT_RES_MGR		L"ResMgr"



#define _LOGGER_ON_

#define SAFE_DELETE(v) {if(v){delete v; v = NULL;}}
#define SAFE_DELETE_ARRAY(v) { if(v) { delete[] v; v = NULL; } }

using namespace std;
typedef basic_string<TCHAR> tstring;