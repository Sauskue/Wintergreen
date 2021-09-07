#include "pch.h"
#include "Window.h"

WNDCLASS Window::wc = {};
bool Window::registered = false;
std::vector<Window*> Window::windows = std::vector<Window*>();

Window::Window():
	x(CW_USEDEFAULT),
	y(CW_USEDEFAULT),
	width(0),
	height(0),
	title(L"Window"),
	window_handle(NULL),
	alive(false)
{
	if (!registered)
	{
		wc =
		{
			CS_OWNDC | CS_HREDRAW | CS_VREDRAW,
			StaticWindowCallback,
			0,
			0,
			GetModuleHandle(NULL),
			NULL,
			NULL,
			NULL,
			NULL,
			L"Default Window Class"
		};
		RegisterClass(&wc);
		registered = true;
	}
	
	if (!width == 0 && !height == 0)
	{
		RECT rc;
		rc.left = 0;
		rc.top = 0;
		rc.right = width;
		rc.bottom = height;
		AdjustWindowRect(&rc, (WS_OVERLAPPEDWINDOW | WS_VISIBLE) & (~WS_THICKFRAME), FALSE);
	}
	window_handle = CreateWindow
	(
		wc.lpszClassName,
		title,
		(WS_OVERLAPPEDWINDOW | WS_VISIBLE) & (~WS_THICKFRAME),
		x,
		y,
		(width != 0) ? width : CW_USEDEFAULT,
		(height != 0) ? height : CW_USEDEFAULT,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		(void*)this
	);
	if (window_handle)
		alive = true;
}

Window::~Window()
{
	alive = false;
}

void Window::ProcessMessages()
{
	while (true)
	{
		if ((int)windows.size() < 1)
			return;
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		for (int i = 0; i < (int)windows.size(); i++)
		{
			if (windows[i]->alive)
			{
				windows[i]->OnUpdate();
				windows[i]->OnDraw();
			}
			else
			{
				windows.erase(windows.begin() + i);
				i--;
			}
		}
	}
}

LRESULT CALLBACK Window::WindowCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_CREATE:
		{
			OnCreate();
			windows.push_back(this);
			break;
		}
		case WM_CLOSE:
		{
			OnDestroy();
			DestroyWindow(hwnd);
			alive = false;
			break;
		}
	}
	return 0;
}

LRESULT CALLBACK Window::StaticWindowCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static bool initialized = false;
	switch (uMsg)
	{
		case WM_NCCREATE:
		{
			Window* window = (Window*)((CREATESTRUCT*)lParam)->lpCreateParams;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG)window);
			initialized = true;
		}
	}
	if (initialized)
		return ((Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA))->WindowCallback(hwnd, uMsg, wParam, lParam);
	else
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
}