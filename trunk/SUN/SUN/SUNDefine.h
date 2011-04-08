#ifndef _SUN_DEFINE_H_
#define _SUN_DEFINE_H_

//#define DLL_API
//#if defined(DLL_EXPORT)
#define DLL_API// __declspec(dllexport)
//#else
//#define DLL_API __declspec(dllimport)
//#endif


//��� ���� �� �κп� ���ؼ��� ���� �� ��ǥ����.
#pragma warning(disable : 4251)			//STL DLL Export ��� ���� �ٽ� ��ǥ����.


#ifndef _WIN32_WINNT		// Windows XP �̻󿡼��� ����� ����� �� �ֽ��ϴ�.                   
#define _WIN32_WINNT 0x0501	// �ٸ� ������ Windows�� �µ��� ������ ������ ������ �ֽʽÿ�.
#endif						

#define WIN32_LEAN_AND_MEAN		// ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

#include <Windows.h>


#include <assert.h>


#include <iostream>
#include <HASH_MAP>
#include <string>

#include <D3D9.h>
#include <D3DX9.h>
#include <dinput.h>


#include "../Include/tinyxml.h"

//�⺻ ���̺�θ�
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