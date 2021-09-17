#include "pch.h"
#include "Application.h"

WNDCLASS Application::Window::wc = { 0 };
bool Application::Window::registered = false;
std::vector<Application::Window*> Application::Window::windows = std::vector<Application::Window*>();
static DWORD window_style = (WS_OVERLAPPEDWINDOW | WS_VISIBLE) & (~WS_THICKFRAME);

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

	RECT rc = {};
	ZeroMemory(&rc, sizeof(RECT));
	if (width != 0 && height != 0)
	{
		rc.left = 0;
		rc.top = 0;
		rc.right = width;
		rc.bottom = height;
		AdjustWindowRect(&rc, window_style, FALSE);
	}

 	window_handle = CreateWindow
	(
		wc.lpszClassName,
		title,
		window_style,
		(x != 0) ? x : CW_USEDEFAULT,
		(y != 0) ? y : CW_USEDEFAULT,
		(width != 0) ? (rc.right - rc.left) : CW_USEDEFAULT,
		(height != 0) ? (rc.bottom - rc.top): CW_USEDEFAULT,
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
			delete windows[i];
			i--;
		}
	}

	if (windows.size() == 0)
	{
		//cleanup
		UnregisterClass(wc.lpszClassName, NULL);
		registered = false;
		return false;
	}
	else
		return true;
}

HWND Application::Window::GetWindowHandle() const
{
	return window_handle;
}

unsigned int Application::Window::GetWidth() const
{
	return width;
}

unsigned int Application::Window::GetHeight() const
{
	return height;
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
			alive = false;
			return 0;
		}
	}

	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

void Application::Window::SetPos(int x, int y)
{
	this->x = x;
	this->y = y;
	
	SetWindowPos(window_handle, HWND_TOP, x, y, width, height, SWP_NOSIZE | SWP_NOZORDER);
}

void Application::Window::SetSize(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	
	RECT rc = {};
	ZeroMemory(&rc, sizeof(RECT));
	rc.left = 0;
	rc.top = 0;
	rc.right = width;
	rc.bottom = height;
	AdjustWindowRect(&rc, window_style, FALSE);

	SetWindowPos(window_handle, HWND_TOP, x, y, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER);
}