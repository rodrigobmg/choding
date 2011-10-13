#include "Precompiled.h"

//#include "sunVectorTest.hpp"
//#include "sunRefTest.h"



int main(int argc, char **argv)
{
	std::cout << "Running main() from gtest_main.cc\n";

	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

