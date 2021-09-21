#pragma once
#include "Application.h"

//window specific events?
class PerlinNoise : public Application
{
public:
protected:
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

class BezierCurve : public Application
{
public:
protected:
private:
	//variables
	ID2D1Factory* d2d1_factory = nullptr;
	ID2D1HwndRenderTarget* d2d1_rt = nullptr;
	ID2D1SolidColorBrush* d2d1_brush = nullptr;
	IDWriteFactory* dwrite_factory = nullptr;
	IDWriteTextFormat* dwrite_tf = nullptr;
	std::vector<D2D1_ELLIPSE> dots = {};
	const int width = 800;
	const int height = 600;
	int index = 0;
	const float radius = 7.0f;
	const float outline_radius = radius * 2;
	float t = 0.5f;
	const float speed = 1.0f;
	bool hide_lines = false;
	bool hide_text = false;
	const float dark_grey = 0.1f;

	//methods
	void OnCreate() override;
	void OnUpdate() override;
	void OnRender() override;
	void OnDestroy() override;
};

class GameOfLife : public Application
{
public:
protected:
private:
	//variables
	static const int width = 800;
	static const int height = 600;
	HDC win_dc = NULL;
	HDC mem_dc = NULL;
	HBITMAP bmp = NULL;

	bool cells[height][width] = { 0 };
	bool cells_next[height][width] = { 0 };
	const int scale = 1;
	unsigned char* bits = nullptr;
	//methods
	void OnCreate() override;
	void OnUpdate() override;
	void OnRender() override;
	void OnDestroy() override;
};

class AStar : public Application
{
public:
protected:
private:
	//variables
	const unsigned int width = 800;
	const unsigned int height = 600;

	//methods
	void OnCreate() override;
	void OnUpdate() override;
	void OnRender() override;
	void OnDestroy() override;
};