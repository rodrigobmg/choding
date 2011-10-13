// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#ifndef _WIN32_WINNT		// Windows XP 이상에서만 기능을 사용할 수 있습니다.                   
#define _WIN32_WINNT 0x0501	// 다른 버전의 Windows에 맞도록 적합한 값으로 변경해 주십시오.
#endif						

#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <assert.h>

#include "util/Utility.h"

#include "tinyXml/tinystr.h"
#include "tinyXml/tinyxml.h"

namespace { using namespace std; }

const int UNITY_NUM = 4;	//Unity_XXX.cpp 갯수.

struct ToLower
{ 
	void operator() (TCHAR& c) const { c = static_cast<TCHAR>(tolower(c)); }
};

class ScopeFile
{
	FILE* fp_;
public:
	ScopeFile(char const * fname, char const * mode)
		: fp_(NULL)
	{
		errno_t rt = fopen_s(&fp_, fname, mode);
		rt;
	}
	~ScopeFile(){ if(fp_)fclose(fp_); fp_=NULL; }
	operator FILE* (){return fp_;}
	FILE* operator ->() {return fp_;}
};

struct project_info_table
{
	wstring project_dir_path;
	wstring project_file_path;
	std::vector< std::string >	filepath_table;
};

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
