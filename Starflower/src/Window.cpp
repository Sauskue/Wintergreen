#include "pch.h"
#include "Window.h"

WNDCLASS Window::wc = {};
bool Window::registered = false;

Window::Window():
	x(CW_USEDEFAULT),
	y(CW_USEDEFAULT),
	width(0),
	height(0),
	title(L"Window"),
	window_handle(NULL)
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
}

Window::~Window(){}

void Window::Init()
{
	//start a thread here
	while (true)
	{
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		OnUpdate();
		OnDraw();
	}
}

LRESULT CALLBACK Window::WindowCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_CREATE:
		{
			OnCreate();
			break;
		}
		case WM_CLOSE:
		{
			OnDestroy();
			DestroyWindow(hwnd);
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