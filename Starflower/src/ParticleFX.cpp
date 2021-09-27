#include "pch.h"
#include "Demos.h"

float v_x_max = 1;
float v_x_min = -1;
float v_y_min = -5;
float v_y_max = -10;
float opacity_max = 1.0f;
float opacity_min = .01f;

ParticleFX::Particle::Particle(float x, float y):
	x(x),
	y(y),
	v_x(((float(rand()) / float(RAND_MAX)) * (v_x_max - v_x_min)) + v_x_min),
	v_y(((float(rand()) / float(RAND_MAX)) * (v_y_max - v_y_min)) + v_y_min),
	opacity(((float(rand()) / float(RAND_MAX))* (opacity_max - opacity_min)) + opacity_min)
{}

void ParticleFX::OnCreate()
{
	SetSize(width, height);
	SetTitle(title);
	std::srand(time(0));
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
	Particle p(width / 2, height - 50);
	p.v_x = (float)GetMouseX() - p.x;
	p.v_y = (float)GetMouseY() - p.y;
	/*float range_vx = ((float(rand()) / float(RAND_MAX)) * (v_x_max - v_x_min)) + v_x_min;
	p.v_x += range_vx;*/
	/*float range_vy = ((float(rand()) / float(RAND_MAX)) * (v_y_max - v_y_min)) + v_y_min;
	p.v_y += range_vy;*/
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
	d2d1_rt->EndDraw();
}

void ParticleFX::OnDestroy()
{
	d2d1_brush->Release();
	d2d1_rt->Release();
	d2d1_factory->Release();
}