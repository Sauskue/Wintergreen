#include "pch.h"
#include "Demos.h"

static ID2D1Factory* d2d1_factory = nullptr;
static ID2D1HwndRenderTarget* d2d1_rt = nullptr;
static ID2D1SolidColorBrush* d2d1_brush = nullptr;
static IDWriteFactory* dwrite_factory = nullptr;
static IDWriteTextFormat* dwrite_tf = nullptr;
static std::vector<D2D1_ELLIPSE> dots = {};
static const int width = 800;
static const int height = 600;
static int index = 0;
static const float radius = 7.0f;
static float t = 0.5f;
static const float speed = 1.0f;
static bool hide_lines = false;
static const float dark_grey = 0.1f;

LRESULT CALLBACK BezierCallback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
		case WM_CREATE:
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
				D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(width, height)),
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
		case WM_LBUTTONDOWN:
		{
			if (wParam & MK_SHIFT)
			{
				D2D1_ELLIPSE dot;
				dot.point = D2D1::Point2F
				(
					(float)GET_X_LPARAM(lParam),
					(float)GET_Y_LPARAM(lParam)
				);
				dot.radiusX = radius;
				dot.radiusY = radius;
				dots.push_back(dot);
				index = (int)dots.size() - 1;
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
		case WM_KEYDOWN:
		{
			if (wParam == VK_SPACE)
				if (hide_lines)
					hide_lines = false;
				else
					hide_lines = true;

			if (wParam == VK_ESCAPE)
			{
				PostQuitMessage(0);
				break;
			}
			//CAN'T GET PROCESS ANY OTHER KEY PRESSES IF THE VECTOR IS EMPTY!!!
			if (dots.size() <= 0)
				break;
			if (wParam == 'A')
			{
				index--;
			}
			else if (wParam == 'D')
			{
				index++;
			}
			else if (wParam == VK_BACK)
			{
				dots.erase(dots.begin() + index);
				if (index >= (int)dots.size())
					index--;
			}
			if (index < 0)
				index = (int)dots.size() - 1;
			else if (index > (int)dots.size() - 1)
				index = 0;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);

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
					d2d1_brush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
					d2d1_rt->FillEllipse(dots.at(0), d2d1_brush);
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
									d2d1_brush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
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
							if(k > 0)
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
			d2d1_brush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
			d2d1_rt->DrawText
			(
				L"SHIFT + CLICK: ADD POINT\nCLICK + DRAG: MOVE POINT\nA/D: TOGGLE ACTIVE POINT\nLEFT/RIGHT: SLIDE POINT\nBACKSPACE: DELETE POINT\nSPACE: TOGGLE LINES\nESCAPE: EXIT",
				156,
				dwrite_tf,
				D2D1::RectF(0.0f, 0.0f, width, 50.0f),
				d2d1_brush,
				D2D1_DRAW_TEXT_OPTIONS_NONE,
				DWRITE_MEASURING_MODE_NATURAL
			);
			d2d1_rt->EndDraw();
			EndPaint(hWnd, &ps);
			break;
		}
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			break;
		}
		default:
		{
			return DefWindowProc(hWnd, Msg, wParam, lParam);
		}
	}
	return 0;
}

int BézierCurveDemo()
{
	WNDCLASS wc =
	{
		CS_OWNDC | CS_HREDRAW | CS_VREDRAW,
		BezierCallback,
		0,
		0,
		GetModuleHandle(NULL),
		NULL,
		NULL,
		NULL,
		NULL,
		L"BezierCurve Window Class"
	};
	RegisterClass(&wc);
	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = width;
	rc.bottom = height;
	AdjustWindowRect(&rc, (WS_OVERLAPPEDWINDOW | WS_VISIBLE) & (~WS_THICKFRAME), FALSE);
	HWND hWnd = CreateWindow
	(
		wc.lpszClassName,
		L"Bezier Curve Demo",
		(WS_OVERLAPPEDWINDOW | WS_VISIBLE) & (~WS_THICKFRAME),
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		NULL
		
	);
	float frame_delta;
	std::chrono::steady_clock::time_point last_time = std::chrono::high_resolution_clock::now();
	while (true)
	{

		std::chrono::duration<float> elapsed_time = std::chrono::high_resolution_clock::now() - last_time;
		frame_delta = elapsed_time.count();
		last_time = std::chrono::high_resolution_clock::now();

		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			if (msg.message == WM_QUIT)
			{
				dots.clear();
				dwrite_tf->Release();
				dwrite_factory->Release();
				d2d1_brush->Release();
				d2d1_rt->Release();
				d2d1_factory->Release();
				DestroyWindow(hWnd);
				UnregisterClass(wc.lpszClassName, NULL);
				return (int)msg.wParam;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if ((GetAsyncKeyState(VK_LBUTTON) >> 15) & !(GetKeyState(VK_SHIFT) >> 15))
		{
			if (dots.size() > 0)
			{
				POINT p;
				GetCursorPos(&p);
				ScreenToClient(hWnd, &p);
				dots.at(index).point = D2D1::Point2F((float)p.x, (float)p.y);
			}
		}
		if (GetKeyState(VK_LEFT) >> 15)
			t -= speed * frame_delta;
		if (GetKeyState(VK_RIGHT) >> 15)
			t += speed * frame_delta;
		if (t <= 0.0f)
			t = 0.0f;
		if (t >= 1.0f)
			t = 1.0f;
		InvalidateRect(hWnd, NULL, TRUE);
	}
	dwrite_tf->Release();
	dwrite_factory->Release();
	d2d1_brush->Release();
	d2d1_rt->Release();
	d2d1_factory->Release();
}

////////////////////////////////////////////////////////////////////

void BezierCurve::OnCreate()
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
	
	if (IsMouseLeftDown() && IsKeyPressed(KeyCode::Shift))
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
	/*if ((GetAsyncKeyState(VK_LBUTTON) >> 15) & !(GetKeyState(VK_SHIFT) >> 15))
	{
		if (dots.size() > 0)
		{
			POINT p;
			GetCursorPos(&p);
			HWND hWnd = GetMainWindowHandle();
			ScreenToClient(hWnd, &p);
			dots.at(index).point = D2D1::Point2F((float)p.x, (float)p.y);
		}
	}*//*
	if (GetKeyState(VK_LEFT) >> 15)
		t -= speed * frame_delta;
	if (GetKeyState(VK_RIGHT) >> 15)
		t += speed * frame_delta;
	if (t <= 0.0f)
		t = 0.0f;
	if (t >= 1.0f)
		t = 1.0f;*/
	
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
			d2d1_brush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
			d2d1_rt->FillEllipse(dots.at(0), d2d1_brush);
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
							d2d1_brush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
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
	d2d1_brush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	d2d1_rt->DrawText
	(
		L"SHIFT + CLICK: ADD POINT\n"
		"CLICK + DRAG: MOVE POINT\n"
		"A/D: TOGGLE ACTIVE POINT\n"
		"LEFT/RIGHT: SLIDE POINT\n"
		"BACKSPACE: DELETE POINT\n"
		"SPACE: TOGGLE LINES\n"
		"ESCAPE: EXIT",
		156,
		dwrite_tf,
		D2D1::RectF(0.0f, 0.0f, (float)width, 50.0f),
		d2d1_brush,
		D2D1_DRAW_TEXT_OPTIONS_NONE,
		DWRITE_MEASURING_MODE_NATURAL
	);
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