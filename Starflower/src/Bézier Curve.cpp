#include "pch.h"
#pragma comment(lib, "d2d1")

ID2D1Factory* d2d1_factory = nullptr;
ID2D1HwndRenderTarget* d2d1_rt = nullptr;
ID2D1SolidColorBrush* d2d1_brush = nullptr;
std::vector<D2D1_ELLIPSE> dots = {};
const int width = 800;
const int height = 600;
int index = 0;
const float radius = 7.0f;
float t = 0.5f;
const float speed = 0.02f;

LRESULT CALLBACK WindowCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
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
				index = dots.size() - 1;
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
		case WM_KEYDOWN:
		{
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
				index = dots.size() - 1;
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
			d2d1_rt->Clear(D2D1::ColorF(D2D1::ColorF::Black));

			//draw points/line at every layer
			std::vector<D2D1_ELLIPSE> temp = dots;
			for (int j = 0; j < dots.size(); j++)
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
					for (int k = 0; k < pois.size(); k++)
					{
						if (pois.size() == 1)
						{
							d2d1_brush->SetColor(D2D1::ColorF(D2D1::ColorF::BlueViolet));
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
								d2d1_rt->FillEllipse(pois.at(k), d2d1_brush);
							}
							else
							{
								d2d1_brush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
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
								d2d1_rt->DrawLine(pois.at(k - 1).point, pois.at(k).point, d2d1_brush);
							}
						}
					}
				}
			}

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
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}
	return 0;
}

int main()
{
	
	WNDCLASS wc =
	{
		CS_OWNDC | CS_HREDRAW | CS_VREDRAW,
		WindowCallback,
		0,
		0,
		GetModuleHandle(NULL),
		NULL,
		NULL,
		NULL,
		NULL,
		L"Window class"
	};
	RegisterClass(&wc);
	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = width;
	rc.bottom = height;
	AdjustWindowRect(&rc, (WS_OVERLAPPEDWINDOW | WS_VISIBLE) & (~WS_THICKFRAME), FALSE);
	HWND hwnd = CreateWindow
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
	while (true)
	{
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			if (msg.message == WM_QUIT)
				return msg.wParam;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if ((GetAsyncKeyState(VK_LBUTTON) >> 15) & !(GetKeyState(VK_SHIFT) >> 15))
		{
			if (dots.size() > 0)
			{
				POINT p;
				GetCursorPos(&p);
				ScreenToClient(hwnd, &p);
				dots.at(index).point = D2D1::Point2F((float)p.x, (float)p.y);
			}
		}
		if (GetKeyState(VK_LEFT) >> 15)
			t -= speed;
		if (GetKeyState(VK_RIGHT) >> 15)
			t += speed;
		if (t <= 0.0f)
			t = 0.0f;
		if (t >= 1.0f)
			t = 1.0f;
		InvalidateRect(hwnd, NULL, TRUE);
	}
	d2d1_brush->Release();
	d2d1_rt->Release();
	d2d1_factory->Release();
}