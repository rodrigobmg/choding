#pragma once


#if defined(DLL_EXPORT)
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif


// ���ʿ��� ��� ����
#pragma warning(disable : 4251)		// STL DLL Export ��� ����
#pragma warning(disable : 4819)		// �ڵ� ������ ��� ����

//�⺻ �ý��� ���
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
