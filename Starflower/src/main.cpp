#include "pch.h"
#include "Demos.h"

// TODO: ParticleFX
// TODO: Pong
// TODO:
// 
// TODO: how do you handle rendering to multiple windows???
// TODO: create graphics framework

int main()
{
	while (true)
	{
		std::cout << "1) Perlin Noise\n";
		std::cout << "2) Bezier Curves\n";
		std::cout << "3) Game of Life\n";
		std::cout << "4) A*\n";
		std::cout << "5) Particle Effects\n";
		std::cout << "6) Pong\n";
		std::cout << "7) Snake\n";

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
				BezierCurve* bcurve_demo = new BezierCurve();
				bcurve_demo->Run();
				delete bcurve_demo;
				break;
			}
			case 3:
			{
				GameOfLife* gol_demo = new GameOfLife();
				gol_demo->Run();
				delete gol_demo;
				break;
			}
			case 4:
			{
				AStar* ax_demo = new AStar();
				ax_demo->Run();
				delete ax_demo;
				break;
			}
			case 5:
			{
				ParticleFX* pfx_demo = new ParticleFX();
				pfx_demo->Run();
				delete pfx_demo;
				break;
			}
			case 6:
			{
				Pong* pong_demo = new Pong();
				pong_demo->Run();
				delete pong_demo;
				break;
			}
			case 7:
			{
				Snake* snake_demo = new Snake();
				snake_demo->Run();
				delete snake_demo;
				break;
			}
			default:
				return 1;
		}
		std::cout << "\n===================================\n";
	}
	return 0;
}