#include <iostream>


void CreateSingleton()
{
	//Test::CreateSingleton();
}

void DestroySingleton()
{
	//Test::DestroySingleton();
}

int main()
{
	CreateSingleton();
	//Test::GetInstance().Run();
	std::cout << "hello world\n" ;

	DestroySingleton();
}