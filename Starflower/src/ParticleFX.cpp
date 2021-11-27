#include "pch.h"
#include "Demos.h"

float v_x_max = .5f;
float v_x_min = -.5f;
float v_y_min = -.5f;
float v_y_max = .5f;
float opacity_max = 1.0f;
float opacity_min = .01f;

ParticleFX::Particle::Particle(float x, float y, float v_x, float v_y):
	x(x),
	y(y),
	v_x(v_x),
	v_y(v_y),
	opacity(((float(rand()) / float(RAND_MAX))* (opacity_max - opacity_min)) + opacity_min)
{}

void ParticleFX::OnCreate()
{
	SetSize(width, height);
	SetTitle(title);
	std::srand((unsigned int)time(0));
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
}

void ParticleFX::OnUpdate()
{
	if (IsKeyPressed(KeyCode::A))
		x -= move_speed;
	if (IsKeyPressed(KeyCode::D))
		x += move_speed;
	if (IsKeyPressed(KeyCode::W))
		y -= move_speed;
	if (IsKeyPressed(KeyCode::S))
		y += move_speed;
	if (IsKeyPressed(KeyCode::Q))
	{
		v_x_max -= .01f;
		if (v_x_max <= 0.2f)
			v_x_max = 0.2f;
		v_x_min += .01f;
		if (v_x_min >= -0.2f)
			v_x_min = -0.2f;
	}
	if (IsKeyPressed(KeyCode::E))
	{
		v_x_max += .01f;
		if (v_x_max >= 2.0f)
			v_x_max = 2.0f;
		v_x_min -= .01f;
		if (v_x_min <= -2.0f)
			v_x_min = -2.0f;

	}
	for (unsigned int i = 0; i < particles.size(); i++)
	{
		Particle& p = particles.at(i);
		p.x += p.v_x;
		p.y += p.v_y;
		if (p.x <= 0 || p.x >= width || p.y <= 0 || p.y >= height)
		{
			particles.erase(particles.begin() + i);
			i--;
		}
	}
	float x_dist = (float)GetMouseX() - x;
	float y_dist = (float)GetMouseY() - y;
	float z_dist = sqrtf((x_dist * x_dist) + (y_dist * y_dist));
	float v_x = x_dist / z_dist;
	float v_y = y_dist / z_dist;
	float speed_multi = 5.0f;
	float range_vx = ((float(rand()) / float(RAND_MAX)) * (v_x_max - v_x_min)) + v_x_min;
	float range_vy = ((float(rand()) / float(RAND_MAX)) * (v_y_max - v_y_min)) + v_y_min;
	Particle p(x, y, (v_x * speed_multi) + range_vx, (v_y * speed_multi) + range_vy);
	p.v_x += range_vx;
	p.v_y += range_vy;
	particles.push_back(p);
}

void ParticleFX::OnRender()
{
	d2d1_rt->BeginDraw();
	d2d1_rt->Clear(D2D1::ColorF(D2D1::ColorF::Black));
	d2d1_brush->SetColor(D2D1::ColorF(D2D1::ColorF::Orange));
	for (const Particle& p : particles)
	{
		D2D1_ELLIPSE e;
		e.point.x = p.x;
		e.point.y = p.y;
		e.radiusX = 10;
		e.radiusY = 10;
		d2d1_brush->SetOpacity(p.opacity);
		d2d1_rt->FillEllipse(e, d2d1_brush);
	}
	D2D1_ELLIPSE e;
	e.point.x = x;
	e.point.y = y;
	e.radiusX = 10;
	e.radiusY = 10;
	d2d1_brush->SetOpacity(1.0f);
	d2d1_rt->FillEllipse(e, d2d1_brush);
	d2d1_rt->EndDraw();
}

void ParticleFX::OnDestroy()
{
	d2d1_brush->Release();
	d2d1_rt->Release();
	d2d1_factory->Release();
}