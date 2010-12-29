#include <Windows.h>
#include <iostream>

#include "../../../chodingModule/Framework/Singleton/CSingleton.h"
#include "Test.h"

int main()
{
	Test::CreateSingleton();
	//Test::GetInstance().Run();
	std::cout << "hello world\n" ;

	Test::DestroySingleton();
}