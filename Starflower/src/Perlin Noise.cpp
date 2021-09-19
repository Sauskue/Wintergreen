#include "pch.h"
#include "Demos.h"

float PerlinNoise::val(float x, float y)
{
	int ix = (int)x;
	int iy = (int)y;

	float fx = x - (float)ix;
	float fy = y - (float)iy;

	ix %= 256;
	iy %= 256;

	Vec2 cv_tl = constant_vectors[iy * 256 + ix];
	Vec2 cv_tr = constant_vectors[iy * 256 + ix + 1];
	Vec2 cv_bl = constant_vectors[(iy + 1) * 256 + ix];
	Vec2 cv_br = constant_vectors[(iy + 1) * 256 + ix + 1];

	Vec2 dv_tl = { fx, fy };
	Vec2 dv_tr = { fx - 1.0f, fy };
	Vec2 dv_bl = { fx, fy - 1.0f };
	Vec2 dv_br = { fx - 1.0f, fy - 1.0f };

	float dot_tl = cv_tl * dv_tl;
	float dot_tr = cv_tr * dv_tr;
	float dot_bl = cv_bl * dv_bl;
	float dot_br = cv_br * dv_br;

	auto smooth = [](float t)
	{
		return t * t * t * (t * (t * 6 - 15) + 10);
	};

	fx = smooth(fx);
	fy = smooth(fy);

	auto lerp = [](float a, float b, float t)
	{
		return a + ((b - a) * t);
	};

	float lerp_l = lerp(dot_tl, dot_bl, fy);
	float lerp_r = lerp(dot_tr, dot_br, fy);
	float lerp_t = lerp(lerp_l, lerp_r, fx);

	return lerp_t;
}

void PerlinNoise::OnCreate()
{
	SetSize(width, height);
	SetPos(200, 200);
	{
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
			D2D1::ColorF(D2D1::ColorF::Black),
			&d2d1_brush
		);
	}

	Vec2 gradients[4] =
	{
		{1.0f, 0.0f},
		{0.0f, -1.0f},
		{-1.0f, 0.0f},
		{0.0f, 1.0f}
	};

	std::srand((unsigned int)std::time(0));
	for (int i = 0; i < 256 * 256 * 2; i++)
	{
		constant_vectors.push_back(gradients[std::rand() % 4]);
	}
	win_dc = GetDC(main_window_handle);

	mem_dc = CreateCompatibleDC(win_dc);

	RECT rc;
	GetClientRect(main_window_handle, &rc);

	BITMAPINFO bmi;
	ZeroMemory(&bmi, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = rc.right - rc.left;
	bmi.bmiHeader.biHeight = rc.top - rc.bottom;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;

	bmp = CreateDIBSection(mem_dc, &bmi, DIB_RGB_COLORS, (void**)&bits, NULL, NULL);
	HGDIOBJ old = SelectObject(mem_dc, bmp);

	BitBlt
	(
		win_dc,
		0,
		0,
		rc.right - rc.left,
		rc.bottom - rc.top,
		mem_dc,
		0,
		0,
		SRCCOPY
	);
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			float intensity = val(x * scale, y * scale);			//[-1, 1]
			intensity += 1.0f;										//[0, 2]
			intensity /= 2.0f;										//[0, 1]
			intensity *= 255;										//[0, 255]
			noise[y][x] = (unsigned char)intensity;
			bits[(y * 4 * width) + (4 * x) + 0] = noise[y][x];		//blue
			bits[(y * 4 * width) + (4 * x) + 1] = noise[y][x];		//green
			bits[(y * 4 * width) + (4 * x) + 2] = noise[y][x];		//red
			bits[(y * 4 * width) + (4 * x) + 3] = (unsigned char)0;	//reserved(set to 0)
		}
	}
	BitBlt
	(
		win_dc,
		0,
		0,
		rc.right - rc.left,
		rc.bottom - rc.top,
		mem_dc,
		0,
		0,
		SRCCOPY
	);
	SelectObject(win_dc, old);
}

void PerlinNoise::OnUpdate()
{

}

void PerlinNoise::OnRender()
{
	HGDIOBJ old = SelectObject(mem_dc, bmp);
	BitBlt
	(
		win_dc,
		0,
		0,
		width,
		height,
		mem_dc,
		0,
		0,
		SRCCOPY
	);
	SelectObject(win_dc, old);
}

void PerlinNoise::OnDestroy()
{
	constant_vectors.clear();
	d2d1_brush->Release();
	d2d1_rt->Release();
	d2d1_factory->Release();
}