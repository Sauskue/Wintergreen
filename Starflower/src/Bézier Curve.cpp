#include "pch.h"
#include "Demos.h"

void BezierCurve::OnCreate()
{
	SetSize(width, height);
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
		L"Arial",
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		15.0f,
		L"en-us",
		&dwrite_tf
	);
}

void BezierCurve::OnUpdate()
{
	static float frame_delta;
	static std::chrono::steady_clock::time_point last_time = std::chrono::high_resolution_clock::now();

	std::chrono::duration<float> elapsed_time = std::chrono::high_resolution_clock::now() - last_time;
	frame_delta = elapsed_time.count();
	last_time = std::chrono::high_resolution_clock::now();
	
	while (!Event::IsBufferEmpty())
	{
		Event e = Event::ReadBuffer();
		if (e.GetType() == Event::Type::KeyPress)
		{
			if (e.GetKeyCode() == KeyCode::Escape)
			{
				Kill();
			}
			if (dots.size() <= 0)
			{
				continue;
			}
			else if (e.GetKeyCode() == KeyCode::Space)
			{
				hide_text = !hide_text;
			}
			else if (e.GetKeyCode() == KeyCode::Control)
			{
				hide_lines = !hide_lines;
			}
			else if(e.GetKeyCode() == KeyCode::A)
			{
				index--;
			}
			else if (e.GetKeyCode() == KeyCode::D)
			{
				index++;
			}
			else if (e.GetKeyCode() == KeyCode::Backspace)
			{
				dots.erase(dots.begin() + index);
				if (index >= (int)dots.size())
					index--;
			}
			if (index < 0)
				index = (int)dots.size() - 1;
			else if (index > (int)dots.size() - 1)
				index = 0;
		}
		else if (e.GetType() == Event::Type::MouseLeftPress)
		{
			if (IsKeyPressed(KeyCode::Shift))
			{
				D2D1_ELLIPSE dot;
				dot.point = D2D1::Point2F
				(
					(float)e.GetX(),
					(float)e.GetY()
				);
				dot.radiusX = radius;
				dot.radiusY = radius;
				dots.push_back(dot);
				index = (int)dots.size() - 1;
			}
		}
	}
	if (IsMouseLeftDown() && !IsKeyPressed(KeyCode::Shift))
	{
		if (dots.size() > 0)
		{
			POINT p;
			GetCursorPos(&p);
			ScreenToClient(main_window_handle, &p);
			dots.at(index).point = D2D1::Point2F((float)p.x, (float)p.y);
		}
	}
	if (IsKeyPressed(KeyCode::Left))
		t -= speed * frame_delta;
	if (IsKeyPressed(KeyCode::Right))
		t += speed * frame_delta;
	if (t <= 0.0f)
		t = 0.0f;
	if (t >= 1.0f)
		t = 1.0f;
	
}

void BezierCurve::OnRender()
{
	d2d1_rt->BeginDraw();
	//clear background
	d2d1_rt->Clear(D2D1::ColorF(D2D1::ColorF::ColorF(dark_grey, dark_grey, dark_grey)));

	//draw points/line at every layer
	std::vector<D2D1_ELLIPSE> temp = dots;
	for (int j = 0; j < (int)dots.size(); j++)
	{
		//if only 1 dot, no need to draw lines
		if (dots.size() == 1)
		{
			if(!hide_lines)
			{
				d2d1_brush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
				D2D1_ELLIPSE e = dots.at(0);
				e.radiusX = outline_radius;
				e.radiusY = outline_radius;
				d2d1_rt->DrawEllipse(e, d2d1_brush);
				d2d1_brush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
				d2d1_rt->FillEllipse(dots.at(0), d2d1_brush);
			}
			continue;
		}
		else
		{
			std::vector<D2D1_ELLIPSE> pois = temp;
			temp.clear();
			for (int k = 0; k < (int)pois.size(); k++)
			{
				if (pois.size() == 1)
				{
					d2d1_brush->SetColor(D2D1::ColorF(D2D1::ColorF::Blue));
					d2d1_rt->FillEllipse(pois.at(k), d2d1_brush);
					continue;
				}
				else
				{
					if (j == 0)
					{
						if (index == k)
						{
							if(!hide_lines)
							{
								d2d1_brush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
								D2D1_ELLIPSE e = pois.at(k);
								e.radiusX = outline_radius;
								e.radiusY = outline_radius;
								d2d1_rt->DrawEllipse(e, d2d1_brush);
								d2d1_brush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
							}
						}
						else
							d2d1_brush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
						if (!hide_lines)
							d2d1_rt->FillEllipse(pois.at(k), d2d1_brush);
					}
					else
					{
						d2d1_brush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
						if (!hide_lines)
							d2d1_rt->DrawEllipse(pois.at(k), d2d1_brush);
					}
					if (k > 0)
					{
						D2D1_ELLIPSE poi;
						poi.point.x = ((1 - t) * pois.at(k - 1).point.x) + (t * pois.at(k).point.x);
						poi.point.y = ((1 - t) * pois.at(k - 1).point.y) + (t * pois.at(k).point.y);
						poi.radiusX = radius;
						poi.radiusY = radius;
						temp.push_back(poi);
						d2d1_brush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
						if (!hide_lines)
							d2d1_rt->DrawLine(pois.at(k - 1).point, pois.at(k).point, d2d1_brush);
					}
				}
			}
		}
	}
	//draw text on top
	if(!hide_text)
	{
		d2d1_brush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
		const wchar_t* text =
			L"SHIFT + CLICK: ADD POINT\n"
			"CLICK + DRAG: MOVE POINT\n"
			"A/D: TOGGLE ACTIVE POINT\n"
			"LEFT/RIGHT: SLIDE POINT\n"
			"BACKSPACE: DELETE POINT\n"
			"SPACE: TOGGLE TEXT\n"
			"CTRL: TOGGLE LINES\n"
			"ESCAPE: EXIT";
		d2d1_rt->DrawText
		(
			text,
			(UINT32)wcslen(text),
			dwrite_tf,
			D2D1::RectF(0.0f, 0.0f, (float)width, 50.0f),
			d2d1_brush,
			D2D1_DRAW_TEXT_OPTIONS_NONE,
			DWRITE_MEASURING_MODE_NATURAL
		);
	}
	d2d1_rt->EndDraw();
}

void BezierCurve::OnDestroy()
{
	dots.clear();
	dwrite_tf->Release();
	dwrite_factory->Release();
	d2d1_brush->Release();
	d2d1_rt->Release();
	d2d1_factory->Release();
}