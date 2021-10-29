#include "pch.h"
#include "Demos.h"

#define PI 3.14159274101257324219f
#define SPEED_MULT 7.5f

void reset(Pong::Puck& puck, int width, int height)
{
	puck.x = (float)width / 2;
	puck.y = (float)height / 2;
	int start_angle_deg = (std::rand() % 91) - 45; //[-45, 45] degrees
	float start_angle_rad = start_angle_deg * (PI / 180.0f); //[22PI/180, 67PI/180] rad
	bool start_dir_x = std::rand() % 2; //0 = left, 1 = right
	puck.dx = cos(start_angle_rad);
	puck.dx *= SPEED_MULT;
	if (start_dir_x == 0)
		puck.dx *= -1;
	puck.dy = sin(start_angle_rad);
	puck.dy *= SPEED_MULT;
}

void Pong::Paddle::Move()
{
	x += dx;
	y += dy;
}

void Pong::Paddle::Show(ID2D1HwndRenderTarget* d2d1_rt, ID2D1SolidColorBrush* d2d1_brush)
{
	d2d1_brush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	D2D1_RECT_F paddle_rect;
	paddle_rect.left = x;
	paddle_rect.right = x + width;
	paddle_rect.top = y;
	paddle_rect.bottom = y + height;
	d2d1_rt->FillRectangle(paddle_rect, d2d1_brush);
}

void Pong::Puck::Move()
{
	x += dx;
	y += dy;
}

void Pong::Puck::Show(ID2D1HwndRenderTarget* d2d1_rt, ID2D1SolidColorBrush* d2d1_brush)
{
	d2d1_brush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	D2D1_ELLIPSE puck_e;
	puck_e.point.x = x;
	puck_e.point.y = y;
	puck_e.radiusX = radius;
	puck_e.radiusY = radius;
	d2d1_rt->FillEllipse(puck_e, d2d1_brush);
}

void Pong::OnCreate()
{
	SetSize(width, height);
	SetTitle(L"Arial");
	HRESULT hr = D2D1CreateFactory
	(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		__uuidof(ID2D1Factory),
		(void**)&d2d1_factory
	);
	hr = d2d1_factory->CreateHwndRenderTarget
	(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(main_window_handle, D2D1::SizeU(width, height)),
		&d2d1_rt
	);
	hr = d2d1_rt->CreateSolidColorBrush
	(
		D2D1::ColorF(D2D1::ColorF::White),
		&d2d1_brush
	);
	hr = DWriteCreateFactory
	(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		(IUnknown**)&dwrite_factory
	);
	hr = dwrite_factory->CreateTextFormat
	(
		L"Consolas",
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		30.0f,
		L"en-us",
		&dwrite_tf
	);

	D2D1_STROKE_STYLE_PROPERTIES ss_prop = {};
	ss_prop.startCap = D2D1_CAP_STYLE_FLAT;
	ss_prop.endCap = D2D1_CAP_STYLE_FLAT;
	ss_prop.dashCap = D2D1_CAP_STYLE_FLAT;
	ss_prop.lineJoin = D2D1_LINE_JOIN_MITER;
	ss_prop.miterLimit = 1.0f;
	ss_prop.dashStyle = D2D1_DASH_STYLE_DASH;
	ss_prop.dashOffset = 0.0f;

	hr = d2d1_factory->CreateStrokeStyle
	(
		ss_prop,
		nullptr,
		NULL,
		&d2d1_ss
	);

	player_paddle.x = 0;
	player_paddle.y = 0;
	player_paddle.dx = 0;
	player_paddle.dy = 0;
	enemy_paddle.x = width - enemy_paddle.width;
	enemy_paddle.y = 0;
	enemy_paddle.dx = 0;
	enemy_paddle.dy = 0;
	puck.x = (float)width / 2;
	puck.y = (float)height / 2;
	
	std::srand((unsigned int)std::time(0));
	reset(puck, width, height);
}

void Pong::OnUpdate()
{
	if (IsKeyPressed(KeyCode::S) || IsKeyPressed(KeyCode::Down))
		player_paddle.dy = paddle_speed;
	if (IsKeyPressed(KeyCode::W) || IsKeyPressed(KeyCode::Up))
		player_paddle.dy = -paddle_speed;
	if (!IsKeyPressed(KeyCode::W) && !IsKeyPressed(KeyCode::Up) && !IsKeyPressed(KeyCode::S) && !IsKeyPressed(KeyCode::Down))
		player_paddle.dy = 0;
	player_paddle.Move();
	if (player_paddle.y <= 0.0f)
		player_paddle.y = 0.0f;
	if (player_paddle.y + player_paddle.height >= height)
		player_paddle.y = height - player_paddle.height;
	puck.Move();
	if (puck.y <= 0 || puck.y >= height)
		puck.dy *= -1;
	
	enum
	{
		NO_HIT,
		PLAYER_HIT,
		ENEMY_HIT
	};

	int hit = NO_HIT;
	if
		(
			puck.x <= player_paddle.x + player_paddle.width
			&& puck.x >= 0
			&& puck.y <= player_paddle.y + player_paddle.height
			&& puck.y >= player_paddle.y
		)
		hit = PLAYER_HIT;
	if
		(
			puck.x >= enemy_paddle.x
			&& puck.x <= width
			&& puck.y >= enemy_paddle.y
			&& puck.y <= enemy_paddle.y + enemy_paddle.height
		)
		hit = ENEMY_HIT;

	if (hit != NO_HIT)
	{
		Paddle* paddle;
		if (hit == PLAYER_HIT)
		{
			paddle = &player_paddle;
			puck.x = player_paddle.x + player_paddle.width + puck.radius;
		}
		else
		{
			paddle = &enemy_paddle;
			puck.x = enemy_paddle.x - (puck.radius * 2);
		}
		float dist = puck.y - (paddle->y + (paddle->height / 2));
		float abs_dist = abs(dist);
		abs_dist /= (paddle->height / 2);
		abs_dist *= (PI / 3.5);
		float new_dx = cos(abs_dist);
		float new_dy = sin(abs_dist);
		if (puck.dx > 0)
		{
			new_dx *= -1;
		}
		if (dist < 0)
			new_dy *= -1;
		puck.dx = new_dx;
		puck.dy = new_dy;
		puck.dx *= SPEED_MULT;
		puck.dy *= SPEED_MULT;
	}

	
	if (puck.x <= 0)
	{
		reset(puck, width, height);
		enemy_score++;
	}
	if (puck.x >= width)
	{
		reset(puck, width, height);
		player_score++;
	}
}

void Pong::OnRender()
{
	d2d1_rt->BeginDraw();
	d2d1_rt->Clear(D2D1::ColorF(D2D1::ColorF::Black));
	D2D1_POINT_2F start_point;
	D2D1_POINT_2F end_point;
	
	start_point.x = (float)width / 2;
	start_point.y = 0;
	end_point.x = (float)width / 2;
	end_point.y = (float)height;
	const float line_width = 3.0f;
	
	d2d1_brush->SetOpacity(0.2f);
	d2d1_rt->DrawLine(start_point, end_point, d2d1_brush, line_width, d2d1_ss);

	d2d1_brush->SetOpacity(1.0f);
	player_paddle.Show(d2d1_rt, d2d1_brush);
	enemy_paddle.Show(d2d1_rt, d2d1_brush);
	puck.Show(d2d1_rt, d2d1_brush);

	std::wstring temp = std::to_wstring(player_score);
	const wchar_t* p_score_txt = temp.c_str();
	std::wstring temp2 = std::to_wstring(enemy_score);
	const wchar_t* e_score_txt = temp2.c_str();

	d2d1_brush->SetOpacity(0.75f);
	d2d1_rt->DrawText
	(
		p_score_txt,
		(UINT32)wcslen(p_score_txt),
		dwrite_tf,
		D2D1::RectF((float)width/4, 0.0f, (float)width/4 + 50.0f, 50.0f),
		d2d1_brush,
		D2D1_DRAW_TEXT_OPTIONS_NONE,
		DWRITE_MEASURING_MODE_NATURAL
	);

	d2d1_rt->DrawText
	(
		e_score_txt,
		(UINT32)wcslen(e_score_txt),
		dwrite_tf,
		D2D1::RectF((3.0f / 4.0f) * (float)width, 0.0f, (3.0f / 4.0f) * (float)width + 50.0f, 50.0f),
		d2d1_brush,
		D2D1_DRAW_TEXT_OPTIONS_NONE,
		DWRITE_MEASURING_MODE_NATURAL
	);

	d2d1_rt->EndDraw();
}

void Pong::OnDestroy()
{
	d2d1_ss->Release();
	dwrite_tf->Release();
	dwrite_factory->Release();
	d2d1_brush->Release();
	d2d1_rt->Release();
	d2d1_factory->Release();
}