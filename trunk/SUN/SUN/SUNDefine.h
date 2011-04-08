#ifndef _SUN_DEFINE_H_
#define _SUN_DEFINE_H_

//#define DLL_API
//#if defined(DLL_EXPORT)
#define DLL_API// __declspec(dllexport)
//#else
//#define DLL_API __declspec(dllimport)
//#endif


//경고 끄기 이 부분에 대해서는 차후 더 살표보자.
#pragma warning(disable : 4251)			//STL DLL Export 경고 추후 다시 살표보자.


#ifndef _WIN32_WINNT		// Windows XP 이상에서만 기능을 사용할 수 있습니다.                   
#define _WIN32_WINNT 0x0501	// 다른 버전의 Windows에 맞도록 적합한 값으로 변경해 주십시오.
#endif						

#define WIN32_LEAN_AND_MEAN		// 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.

#include <Windows.h>


#include <assert.h>


#include <iostream>
#include <HASH_MAP>
#include <string>

#include <D3D9.h>
#include <D3DX9.h>
#include <dinput.h>


#include "../Include/tinyxml.h"

//기본 라이브로리
#pragma comment(lib, "D3D9.lib")
#pragma comment(lib, "D3DX9.lib")
#pragma comment(lib, "DINPUT8.lib")

#ifdef _DEBUG
	#pragma comment(lib, "../Library/TinyXMLStaticMDd.lib")
#else
	#pragma comment(lib, "../Library/TinyXMLStaticMD.lib")
#endif

#include <boost/foreach.hpp>
#include <boost/pool/pool.hpp>


using namespace std;
typedef basic_string<TCHAR> tstring;


#endif