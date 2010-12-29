#include <iostream>
#include "CommonHeader.h"
#include "../Framework/ResManager/ResManager.h"

void CreateSingleton()
{
	CResManager< Resource<LPDIRECT3DTEXTURE9> >::CreateSingleton();//	g_mgrTexture;
	//CResManager::CreateSingleton();
}

void DestroySingleton()
{
	CResManager< Resource<LPDIRECT3DTEXTURE9> >::DestroySingleton();
}

int main()
{
	CreateSingleton();
	//Test::GetInstance().Run();
	std::cout << "hello world\n" ;
	//CResManager::GetInstance().Run();
	
	std::wstring str;
	str = L"hi";
	CResManager< Resource<LPDIRECT3DTEXTURE9> >::GetInstance().Load( str );

	DestroySingleton();
}