#pragma once
#include "choding_stdafx.h"
#include "ResManager.h"


template<> LPDIRECT3DTEXTURE9 resMgr::Load( wchar_t* filename )
{
	FILE *fpFile = NULL;

	//	읽기전용 바이너리
	//fpFile = fopen( filename , "rb");

	//	파일이 없으면 0을 리턴
	if(fpFile == NULL)
		return NULL;

	//	파일 사이즈 체크
	int32_t iFileSize = GetFileSize(fpFile , NULL );

	D3DXIMAGE_INFO kImageInfo;
	//HRESULT hr = D3DXGetImageInfoFromFileInMemory((void*)fpFile, iFileSize , &kImageInfo);
	HRESULT hr;
	if(D3D_OK != hr)
	{
		return NULL;
	}
	

	printf( "Load Texture" );

	fclose(fpFile);							// 파일을 열었으니 닫아주자

	return NULL;
}
