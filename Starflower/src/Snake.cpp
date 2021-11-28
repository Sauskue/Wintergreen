#include "pch.h"
#include "Demos.h"

void Snake::OnCreate()
{
	SetTitle(L"Snake");
	SetSize(800, 600);
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

	std::srand((unsigned int)std::time(0));
}

void Snake::OnUpdate()
{
	if(IsKeyPressed(KeyCode::Space))
		if (!playing)
		{
			player_x = std::rand() % width;
			player_x -= (player_x % spacing);
			player_y = std::rand() % height;
			player_y -= (player_y % spacing);
			playing = true;
		}

	if (playing)
	{

	}
}

void Snake::OnRender()
{
	d2d1_rt->BeginDraw();
	d2d1_rt->Clear(D2D1::ColorF(D2D1::ColorF::Gray));
	d2d1_brush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
	d2d1_brush->SetOpacity(1.0f);
	for (unsigned int i = 0; i < width; i += spacing)
	{
		D2D1_POINT_2F p1, p2;
		p1.x = (float)i;
		p1.y = 0.0f;
		p2.x = (float)i;
		p2.y = (float)height;
		d2d1_rt->DrawLine(p1, p2, d2d1_brush);
	}
	for (unsigned int j = 0; j < height; j += spacing)
	{
		D2D1_POINT_2F p1, p2;
		p1.x = 0.0f;
		p1.y = (float)j;
		p2.x = (float)width;
		p2.y = (float)j;
		d2d1_rt->DrawLine(p1, p2, d2d1_brush);
	}

	std::wstring temp = std::to_wstring(score);
	const wchar_t* score_text = temp.c_str();

	d2d1_brush->SetOpacity(0.50f);
	d2d1_brush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	d2d1_rt->DrawText
	(
		score_text,
		(UINT32)wcslen(score_text),
		dwrite_tf,
		D2D1::RectF(0.0f, 0.0f, (float)width, 50.0f),
		d2d1_brush,
		D2D1_DRAW_TEXT_OPTIONS_NONE,
		DWRITE_MEASURING_MODE_NATURAL
	);


	if (!playing)
	{
		const wchar_t* start_text = L"PRESS SPACE TO START";
		d2d1_rt->DrawText
		(
			start_text,
			(UINT32)wcslen(start_text),
			dwrite_tf,
			D2D1::RectF((float)width * 2 / 7, float(height) * 2 / 5, (float)width * 2 / 7 + 350.0f, (float)height * 2 / 5 + 50.0f),
			d2d1_brush,
			D2D1_DRAW_TEXT_OPTIONS_NONE,
			DWRITE_MEASURING_MODE_NATURAL

		);
	}
	else
	{
		d2d1_brush->SetOpacity(1.0f);
		d2d1_brush->SetColor(D2D1::ColorF(D2D1::ColorF::LimeGreen));
		D2D1_RECT_F rect;
		rect.left = player_x;
		rect.top = player_y;
		rect.right = player_x + spacing;
		rect.bottom = player_y + spacing;
		d2d1_rt->FillRectangle(rect, d2d1_brush);
	}
	d2d1_rt->EndDraw();
}

void Snake::OnDestroy()
{
	d2d1_ss->Release();
	dwrite_tf->Release();
	dwrite_factory->Release();
	d2d1_brush->Release();
	d2d1_rt->Release();
	d2d1_factory->Release();
}