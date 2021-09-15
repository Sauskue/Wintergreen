#pragma once
#include "Application.h"

//window specific events?
class PerlinNoise : public Application
{
private:
	//variables
	static const int width = 800;
	static const int height = 600;

	ID2D1Factory* d2d1_factory = nullptr;
	ID2D1HwndRenderTarget* d2d1_rt = nullptr;
	ID2D1SolidColorBrush* d2d1_brush = nullptr;
	unsigned char noise[height][width] = { 0 };
	unsigned char* bits = nullptr;
	HDC mem_dc = NULL;
	HDC win_dc = NULL;
	HBITMAP bmp = NULL;
	const float scale = 0.02f;
	struct Vec2
	{
		float x = 0;
		float y = 0;
		Vec2(float x, float y) :
			x(x), y(y)
		{}
		float operator*(const Vec2& other)
		{
			return ((x * other.x) + (y * other.y));
		}
	};
	std::vector<Vec2> constant_vectors;

	//methods
	float val(float x, float y);

	void OnCreate() override;

	void OnUpdate() override;

	void OnRender() override;

	void OnDestroy() override;
};