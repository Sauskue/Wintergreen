#include "pch.h"
#include "Application.h"

WNDCLASS Application::Window::wc = { 0 };
bool Application::Window::registered = false;
std::vector<Application::Window*> Application::Window::windows = std::vector<Application::Window*>();

Application::Window::Window():
	Window(-1, -1, 0, 0)
{}

Application::Window::Window(int x, int y):
	Window(x, y, 0, 0)
{}

Application::Window::Window(int x, int y, int width, int height):
	x(x),
	y(y),
	width(width),
	height(height)
{
	if (!registered)
	{
		wc =
		{
			CS_OWNDC | CS_HREDRAW | CS_VREDRAW,
			StaticWindowProc,
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
		(x != 0) ? x : CW_USEDEFAULT,
		(y != 0) ? y : CW_USEDEFAULT,
		(width != 0) ? width : CW_USEDEFAULT,
		(height != 0) ? height : CW_USEDEFAULT,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		(void*)this
	);

	if (window_handle)
	{
		windows.push_back(this);
		alive = true;
	}
}

Application::Window::~Window()
{
	windows.erase(windows.begin() + index);
	DestroyWindow(window_handle);
	alive = false;
}

bool Application::Window::Update()
{
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	for (unsigned int i = 0; i < windows.size(); i++)
	{
		windows[i]->index = i;
		if (!windows[i]->alive)
		{
			windows[i]->~Window();
			i--;
		}
	}

	if (windows.size() == 0)
		return false;
	else
		return true;
}

LRESULT CALLBACK Application::Window::StaticWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	static bool initialized = false;
	switch (Msg)
	{
		case WM_NCCREATE:
		{
			Window* window = (Window*)((CREATESTRUCT*)lParam)->lpCreateParams;
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)window);
			initialized = true;
		}
	}
	if (initialized)
		return ((Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WindowProc(hWnd, Msg, wParam, lParam);
	else
		return DefWindowProc(hWnd, Msg, wParam, lParam);
}

LRESULT CALLBACK Application::Window::WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
		case WM_CLOSE:
		{
			this->~Window();
			return 0;
		}
	}

	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

void Application::Window::SetPos(int x, int y)
{
	this->x = x;
	this->y = y;
}

void Application::Window::SetSize(int width, int height)
{
	this->width = width;
	this->height = height;
}