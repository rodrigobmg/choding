#ifndef _SNOWBOARD_PCH_H_
#define _SNOWBOARD_PCH_H_

#ifndef WINVER    // Windows XP 이상에서만 기능을 사용할 수 있습니다.
#define WINVER 0x0501  // 다른 버전의 Windows에 맞도록 적합한 값으로 변경해 주십시오.
#endif

#ifndef _WIN32_WINNT  // Windows XP 이상에서만 기능을 사용할 수 있습니다.                   
#define _WIN32_WINNT 0x0501 // 다른 버전의 Windows에 맞도록 적합한 값으로 변경해 주십시오.
#endif      

#include <Windows.h>
#include <winbase.h>
#include "d3dx9.h"
#include <assert.h>
#include <tchar.h>
#include <String>
#include <Strsafe.h>
#include <vector>
#include <queue>
#include <list>
#include <map>
#include <algorithm>
#include <stdlib.h>
#include <hash_map>
#include <wtypes.h>

// boost
#include <boost/pool/pool.hpp>
#include <boost/pool/poolfwd.hpp>
#include <boost/pool/object_pool.hpp>
#include <boost/pool/singleton_pool.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/tuple/tuple.hpp>

// snowboard library macro & type
#include "Snowboard_Macro.h"
#include "Snowboard_Type.h"

//mem object
#include "MemObject/GdsMemObject.h"

//component
#include "Component/GdsComponent.h"
//visual
#include "Component/Comvisual/GdsComVisual.h"
#include "Component/Comvisual/GdsComVisualRect.h"
#include "Component/Comvisual/GdsComVisualMesh.h"

//animaion
#include "Component/ComAnimation/GdsComAnimationFamily.h"
#include "Component/ComAnimation/GdsComMoveable.h"

//component-maker
#include "Component/GdsComponentMaker.h"

//gameobject
#include "GameObject/GdsGameObject.h"


//프레임 워크
#include "Framework/Util/Utility.h"
#include "Framework/Singleton/Singleton.h"
#include "Framework/FrameMemory/GdsFrameMemory.h"
#include "Framework/FileSystem/GdsFile.h"
#include "Framework/PerformanceCheck/Performance.h"
#include "Framework/ThreadPool/GdsSynchronize.h"
#include "Framework/ThreadPool/GdsSyncCriticalSection.h"
#include "Framework/ThreadPool/GdsThread.h"
#include "Framework/ThreadPool/GdsBGThread.h"
#include "Framework/ThreadPool/GdsThreadPool.h"
#include "Framework/Logger/console_logger.h"
#include "Framework/Logger/html_logger.h"
#include "Framework/Logger/output_logger.h"
#include "Framework/Logger/logger.h"
#include "Framework/InputSystem/GdsMouse.h"
#include "Framework/InputSystem/GdsKeyboard.h"
#include "Framework/InputSystem/GdsInputSystem.h"

//renderer
#include "Renderer/GdsRendererBase.h"
#include "Renderer/GdsRendererDX9.h"
#include "Renderer/GdsRenderObject.h"
#include "Renderer/GdsRenderFrame.h"
#include "Renderer/GdsRenderStateGroup.h"
#include "Renderer/GdsRendererManager.h"

//resource manager
#include "ResourceManager/Type/GdsResBaseType.h"
#include "ResourceManager/Type/GdsIndexBuffer.h"
#include "ResourceManager/Type/GdsPixelshader.h"
#include "ResourceManager/Type/GdsResTexture.h"
#include "ResourceManager/Type/GdsVertexBuffer.h"
#include "ResourceManager/Type/GdsVertexShader.h"
#include "ResourceManager/GdsResMgr.h"

//camera 
#include "Camera/GdsFrustum.h"
#include "Camera/GdsCamera.h"
#include "Camera/GdsCameraManagerDX9.h"

//terrain
#include "Terrain/GdsTerrain_Triangle.h"
#include "Terrain/GdsTerrain_Tile.h"
#include "Terrain/GdsTerrain.h"

//scene
#include "Scene/GdsNode.h"

//snowboard main
#include "Snowboard.h"

#pragma comment(lib,"d3d9.lib")

#endif
