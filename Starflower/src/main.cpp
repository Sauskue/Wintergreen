#include "pch.h"
#include "Demos.h"


//TODO: add keyboard/mouse events
//TODO: multi-window functionality(window-specific event pumps?)


int main()
{
	while (true)
	{
		std::cout << "1) Perlin Noise\n";
		std::cout << "2) Bezier Curves\n";
		std::cout << "3) Game of Life\n";

		std::cout << "\nPick a demo: ";
		int i = 0;
		std::cin >> i;
		switch (i)
		{
			case 1:
			{
				PerlinNoise* pnoise_demo = new PerlinNoise();
				pnoise_demo->Run();
				delete pnoise_demo;
				break;
			}
			case 2:
			{
				break;
			}
			case 3:
			{
				break;
			}
			default:
				return 1;
		}
		std::cout << "\n===================================\n";
	}
	return 0;
}