#pragma once


#if defined(DLL_EXPORT)
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif


// ���ʿ��� ��� ����
#pragma warning(disable : 4251)		// STL DLL Export ��� ����

//�⺻ �ý��� ���
#include <Windows.h>

//#include <utility>
#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <tchar.h>


#include <HASH_MAP>
#include <String>		