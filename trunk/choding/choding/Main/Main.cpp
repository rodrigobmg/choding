//#include <Windows.h>
#include <iostream>

//#include "../../../chodingModule/Framework/Singleton/CSingleton.h"
#include "Test.h"

void CreateSingleton()
{
	Test::CreateSingleton();
}

void DestroySingleton()
{
	Test::DestroySingleton();
}

int main()
{
	CreateSingleton();
	Test::GetInstance().Run();
	std::cout << "hello world\n" ;

	DestroySingleton();
}