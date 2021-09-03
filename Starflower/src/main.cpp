#include "pch.h"
#include "Demos.h"

int main()
{
	std::cout << "1)Bezeir Curves" << std::endl;
	std::cout << "2)Perlin Noise" << std::endl;
	std::cout << "3)Test Code" << std::endl;

	std::cout << std::endl;
	std::cout << "Select Demo: ";
	int i;
	std::cin >> i;
	switch (i)
	{
		case 1:
			return BézierCurveDemo();
		case 2:
			return PerlinNoiseDemo();
		case 3:
		{
			
			return 0;
		}
		default:
			return 0;
	}
}