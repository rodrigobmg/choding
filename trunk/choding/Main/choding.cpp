// choding.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "choding_stdafx.h"

#include "../Framework/ResourceManager/ResManager.h"

#include "../../chodingModule/Snowboard/Framework/Snowboard.h"


int _tmain(int argc, _TCHAR* argv[])
{

	//g_pTextureResMgr::->SetRootDir( "hi" );
	resMgr_Texture::CreateSingleton();


	//CSnowboard* pSnow = new CSnowboard;
	//pSnow->Init( NULL );
	//HWND hWnd;
	//snowboard.Init( hWnd );
	
	std::wstring wstr = L"�ϸ� wb";
	std::string	str  = "�ϸ� mb";

	wstr = util::conversion_string::mb2wc( str.c_str() , util::conversion_string::Korea );

	resMgr_Texture::GetInstance().SetRootDir( L"hi" );
	//res_Texture_ptr pTexture = resMgr_Texture::GetInstance().Load( L"hi" );
	res_Texture_ptr pTexture = resMgr_Texture::GetInstance().Get();
	if ( pTexture != NULL )
	{
		printf( "Load Success\n");
	}

	resMgr_Texture::DestroySingleton();

	return 0;
}

