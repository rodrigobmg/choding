#ifndef _GDS_H_STDAFX_
#define _GDS_H_STDAFX_

#ifndef WINVER    // Windows XP 이상에서만 기능을 사용할 수 있습니다.
#define WINVER 0x0501  // 다른 버전의 Windows에 맞도록 적합한 값으로 변경해 주십시오.
#endif

#ifndef _WIN32_WINNT  // Windows XP 이상에서만 기능을 사용할 수 있습니다.                   
#define _WIN32_WINNT 0x0501 // 다른 버전의 Windows에 맞도록 적합한 값으로 변경해 주십시오.
#endif      

#include <Windows.h>
#include <WinBase.h>
#include <d3dx9.h>
#include <ASSERT.h>
#include <d3d9.h>
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

#include <boost/pool/pool.hpp>
#include <boost/pool/poolfwd.hpp>
#include <boost/pool/singleton_pool.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/tuple/tuple.hpp>

using namespace std;

typedef char				int8_t;
typedef short				int16_t;
typedef int					int32_t;
typedef __int64				int64_t;

typedef unsigned char		uint8_t;
typedef unsigned short		uint16_t;
typedef unsigned int		uint32_t;
typedef unsigned __int64	uint64_t;


#define CORE_RENDERER		L"RendererCore"
#define OBJECT_CAMERA_BASE	L"CameaCoreBase"
#define CORE_RESOURCE		L"ResourceCore"
#define CORE_CAMMGR_DX9		L"CamManagerDX9"

#define OBJECT_BASE			L"Base"
#define OBJECT_BASE_CORE	L"CoreBase"
#define OBJECT_RENDERBASE	L"RenderBase"
#define OBJECT_RENDEROBJECT	L"RenderObject"
#define OBJECT_RENDERFRAME	L"RenderFrame"
#define OBJECT_RENDERSTATEGROUP	L"RenderStateGroup"
#define OBJECT_RENDERERDX9	L"RendererDX9"
#define OBJECT_COREMGR		L"CoreMgr"
#define OBJECT_RES_BASE		L"ResBase"
#define OBJECT_RES_TEXTURE	L"ResTypeTexture"
#define OBJECT_RES_MD2		L"ResTypeMD2"
#define OBJECT_RES_ASE		L"ResTypeASE"
#define OBJECT_RES_MGR		L"ResMgr"
#define OBJECT_CAMMGR_DX9	L"CamMgrDx9"

#define OBJECT_SYNCCHRONIZE	L"GdsSynchronize"
#define OBJECT_CRITICALSECTION	L"CriticalSection"
#define OBJECT_THREAD		L"SnowThread"
#define OBJECT_THREADPOOL	L"ThreadPool"
#define OBJECT_BGTHREAD		L"BGThread"

#define OBJECT_AVOBJECT		L"GdsAVObject"
#define OBJECT_NODE_BASE	L"GdsNodeBase"
#define OBJECT_NODE			L"GdsNode"
#define OBJECT_NODE_MESH	L"Mesh"
#define OBJECT_NODE_CHILDMESH	L"ChildMesh"
#define OBJECT_NODE_CAMERA	L"CameraNode"

#define OBJECT_TERRAIN		L"Terrain"
#define OBJECT_MATRIX3		L"Matrix3"
#define OBJECT_FRUSTUM		L"Frustum"
#define OBJECT_TRANSFORM	L"Transform"
#define OBJECT_VECTOR3		L"Vector3"

#define OBJECT_PROPERTYSTATE	L"PropertyState"
#define OBJECT_PROPERTY			L"Property"
#define OBJECT_PROPERTY_TEXTURE	L"Texture_Property"
#define OBJECT_PROPERTY_POLYGON	L"Polygon_Property"

#include "../System/Singleton/Singleton.h"
#include "Snowboard_macro.h"
#include "../System/Util/Utility.h"

struct GDSVERTEX
{
	enum _FVF { FVF=(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1) };
	D3DXVECTOR3		p;
	D3DXVECTOR3		n;
	D3DXVECTOR2		t;
};

struct GDSINDEX
{
	WORD _0 , _1, _2;
	GDSINDEX(){};
	GDSINDEX( WORD __0 , WORD __1 , WORD __2 ):_0(__0),_1(__1),_2(__2)
	{}
};

//메모리 릭 탐지
#define _CRTDBG_MAP_ALLOC

#ifdef _CRTDBG_MAP_ALLOC

	#define ImplementBoostPool(class) //boost::pool<> class::bpool(sizeof(class));
	#define DeclareBoostPool //\

	#include <crtdbg.h>
	#define new new(_NORMAL_BLOCK , __FILE__ , __LINE__ )
	static class MemoryLeakCheck
	{
	public:
		MemoryLeakCheck()
		{
			_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
		}
		~MemoryLeakCheck()
		{
			_ASSERTE( _CrtCheckMemory() );
		}
	} MemoryLeak;

#else
	
	// 부스트풀 메모리
// 	#define ImplementBoostPool(class) \
// 	struct class##tag {};	\
// 	boost::singleton_pool< class##tag , sizeof( class ) > class##pool;	\
	

	#define DeclareBoostPool( class ) \
		void* operator new(size_t s) \
	{ \
		return class##bpool.malloc(); \
	} \
		void operator delete(void *p) \
	{ \
		class##bpool.free(p); \
	} \
 	private: \
	struct class##tag {};	\
	boost::singleton_pool< class##tag , sizeof( class ) > class##pool;	\


#endif

#endif