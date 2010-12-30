#pragma once
#include "choding_stdafx.h"
#include "ResManager.h"


template<> LPDIRECT3DTEXTURE9 resMgr::Load( wchar_t* filename )
{
	FILE *fpFile = NULL;

	//	�б����� ���̳ʸ�
	//fpFile = fopen( filename , "rb");

	//	������ ������ 0�� ����
	if(fpFile == NULL)
		return NULL;

	//	���� ������ üũ
	int32_t iFileSize = GetFileSize(fpFile , NULL );

	D3DXIMAGE_INFO kImageInfo;
	//HRESULT hr = D3DXGetImageInfoFromFileInMemory((void*)fpFile, iFileSize , &kImageInfo);
	HRESULT hr;
	if(D3D_OK != hr)
	{
		return NULL;
	}
	

	printf( "Load Texture" );

	fclose(fpFile);							// ������ �������� �ݾ�����

	return NULL;
}
