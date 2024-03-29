#pragma once
#include "Application.h"

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
	//classes
	struct Node
	{
	public:
		Node(int x, int y);
		
		int x;
		int y;
		float local_goal;
		float global_goal;
		bool visited;
		Node* parent;
		std::vector<Node*> neighbors;

		bool operator<(const Node& other);
	protected:
	private:
	};
protected:
private:

	//variables
	const unsigned int width = 800;
	const unsigned int height = 600;

	static const int node_radius = 10;
	static const int node_border = node_radius * 10;
	static const int selection_node_radius = node_radius + 5;

	bool quick_mode = false;

	Node* start_node;
	Node* end_node;
	Node* selected_node;
	
	std::vector<Node*> nodes;
	std::vector<Node*> open_list;
	std::vector<Node*> closed_list;

	ID2D1Factory* d2d1_factory = nullptr;
	ID2D1HwndRenderTarget* d2d1_rt = nullptr;
	ID2D1SolidColorBrush* d2d1_brush = nullptr;
	IDWriteFactory* dwrite_factory = nullptr;
	IDWriteTextFormat* dwrite_tf = nullptr;

	//methods
	void OnCreate() override;
	void OnUpdate() override;
	void OnRender() override;
	void OnDestroy() override;

	void RunAlgorithm();
};

class ParticleFX : public Application
{
public:
	struct Particle
	{
	public:
		Particle(float x, float y, float v_x, float v_y);
		
		float x;
		float y;
		float v_x;
		float v_y;

		float opacity;
	protected:
	private:
	};

protected:
private:
	const unsigned int width = 600;
	const unsigned int height = 600;
	const wchar_t* title = L"Particle Effects";

	float x = (float)width / 2;
	float y = (float)height - 50;

	const float move_speed = 2.0f;

	std::vector<Particle> particles = std::vector<Particle>();

	ID2D1Factory* d2d1_factory = nullptr;
	ID2D1HwndRenderTarget* d2d1_rt = nullptr;
	ID2D1SolidColorBrush* d2d1_brush = nullptr;

	void OnCreate() override;
	void OnUpdate() override;
	void OnRender() override;
	void OnDestroy() override;
};

class Pong : public Application
{
public:
	struct Paddle
	{
		void Show(ID2D1HwndRenderTarget*, ID2D1SolidColorBrush*);
		void Move();

		float x;
		float y;
		float dx;
		float dy;
		const float width = 15.0f;
		const float height = 80.0f;
	};

	struct Puck
	{
		void Show(ID2D1HwndRenderTarget*, ID2D1SolidColorBrush*, D2D1::ColorF color = D2D1::ColorF::White);
		void Move();

		float x;
		float y;
		float dx;
		float dy;
		const float radius = 9.0f;

		Puck& operator=(const Puck& other);
	};
protected:
private:
	//variables
	Paddle player_paddle;
	Paddle enemy_paddle;
	Puck puck;
	Paddle* last_hit;

	const unsigned int width = 800;
	const unsigned int height = 600;
	const float paddle_speed = 7.0f;

	int player_score = 0;
	int enemy_score = 0;

	ID2D1Factory* d2d1_factory = nullptr;
	ID2D1HwndRenderTarget* d2d1_rt = nullptr;
	ID2D1SolidColorBrush* d2d1_brush = nullptr;
	IDWriteFactory* dwrite_factory = nullptr;
	IDWriteTextFormat* dwrite_tf = nullptr;
	ID2D1StrokeStyle* d2d1_ss = nullptr;
	//methods
	void OnCreate() override;
	void OnUpdate() override;
	void OnRender() override;
	void OnDestroy() override;

	void Reset();
	void UpdatePlayerPaddle();
	void UpdateEnemyPaddle();
	void UpdatePuck();
};