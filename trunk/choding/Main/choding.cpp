// choding.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
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
	
	std::wstring wstr = L"니마 wb";
	std::string	str  = "니마 mb";

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

