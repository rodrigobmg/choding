#pragma once


#if defined(DLL_EXPORT)
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif


// 불필요한 경고 끄기
#pragma warning(disable : 4251)		// STL DLL Export 경고 무시
#pragma warning(disable : 4819)		// 코드 페이지 경고 무시

//기본 시스템 헤더
#include <Windows.h>

//#include <utility>
#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <tchar.h>


#include <HASH_MAP>
#include <String>		




#include <D3D9.h>
#include <D3DX9.h>



#pragma comment(lib, "D3D9.lib")
#pragma comment(lib, "D3DX9.lib")
