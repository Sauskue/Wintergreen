#include "pch.h"
#include "Demos.h"

int main()
{
	while(true)
	{
		std::cout << "1)Bezeir Curves" << std::endl;
		std::cout << "2)Perlin Noise" << std::endl;
		std::cout << "3)Exit" << std::endl;

		std::cout << std::endl;
		std::cout << "Select Demo: ";
		int i;
		int exit_code = 0;
		std::cin >> i;
		switch (i)
		{
			case 1:
				exit_code = BézierCurveDemo();
				break;
			case 2:
				exit_code = PerlinNoiseDemo();
				break;
			default:
				return exit_code;
		}
	}
}