#pragma once
#include "Application.h"


class PerlinNoise : public Application
{
private:
	void OnCreate() override
	{

	}

	void OnUpdate() override
	{
		std::cout << "Update called\n";
	}

	void OnRender() override
	{
		std::cout << "Render called\n";
	}

	void OnDestroy() override
	{

	}
};