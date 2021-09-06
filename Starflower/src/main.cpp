#include "pch.h"
#include "Demos.h"

int main()
{
	while(true)
	{
		std::cout << "1)Bezier Curves" << std::endl;
		std::cout << "2)Perlin Noise" << std::endl;
		std::cout << "3)Conway's Game of Life" << std::endl;
		std::cout << "4)Exit" << std::endl;

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
			case 3:
				exit_code = GameOfLife();
				break;
			default:
				return exit_code;
		}
	}
}