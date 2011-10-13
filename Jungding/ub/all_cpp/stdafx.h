// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#ifndef _WIN32_WINNT		// Windows XP �̻󿡼��� ����� ����� �� �ֽ��ϴ�.                   
#define _WIN32_WINNT 0x0501	// �ٸ� ������ Windows�� �µ��� ������ ������ ������ �ֽʽÿ�.
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

const int UNITY_NUM = 4;	//Unity_XXX.cpp ����.

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

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
