#ifndef _GDS_H_STDAFX_
#define _GDS_H_STDAFX_

#include <Windows.h>
#include <d3dx9.h>
#include <assert.h>
#include <d3d9.h>
#include <tchar.h>
#include <String>
#include <Strsafe.h>
#include <vector>
#include <queue>
#include <list>

#include <boost/pool/pool.hpp>
#include <boost/shared_ptr.hpp>

typedef char				int8_t;
typedef short				int16_t;
typedef int					int32_t;
typedef __int64				int64_t;

typedef unsigned char		uint8_t;
typedef unsigned short		uint16_t;
typedef unsigned int		uint32_t;
typedef unsigned __int64	uint64_t;


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

#define OBJECT_SYNCCHRONIZE	L"GdsSynchronize"
#define OBJECT_CRITICALSECTION	L"CriticalSection"
#define OBJECT_THREAD		L"SnowThread"
#define OBJECT_THREADPOOL	L"ThreadPool"
#define OBJECT_BGTHREAD		L"BGThread"

#define OBJECT_NODE_BASE	L"GdsNodeBase"
#define OBJECT_NODE_MESH	L"Mesh"
#define OBJECT_NODE_CHILDMESH	L"ChildMesh"

#include "Snowboard_macro.h"
#include "../Utility/Utility.h"


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
	#define ImplementBoostPool(class) boost::pool<> class::bpool(sizeof(class));
	#define DeclareBoostPool \
		void* operator new(size_t s) \
	{ \
		return bpool.malloc(); \
	} \
		void operator delete(void *p) \
	{ \
		bpool.free(p); \
	} \
	private: \
		static boost::pool<> bpool; \


#endif

#endif