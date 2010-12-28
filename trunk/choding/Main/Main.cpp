#include <iostream>
#include "Test.h"

void CreateInstance()
{
	CTest::CreateInstance();
}

void DestroyInstance()
{
	CTest::DestroyInstance();
}

int main()
{
	std::cout << "hello world\n";

	CreateInstance();
	DestroyInstance();
}