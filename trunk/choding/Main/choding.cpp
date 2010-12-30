// choding.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "choding_stdafx.h"

#include "../Framework/ResourceManager/ResManager.h"

int _tmain(int argc, _TCHAR* argv[])
{
	printf( "hello world\n");
	
	//g_pTextureResMgr::->SetRootDir( "hi" );
	resMgr_Texture::CreateSingleton();


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

