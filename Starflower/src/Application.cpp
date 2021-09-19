#include "pch.h"
#include "Application.h"

static WNDCLASS wc = { 0 };
static DWORD window_style = (WS_OVERLAPPEDWINDOW | WS_VISIBLE) & (~WS_THICKFRAME);
bool Application::registered = false;

LRESULT CALLBACK Application::StaticWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	static bool initialized = false;
	switch (Msg)
	{
		case WM_NCCREATE:
		{
			Application* app = (Application*)((CREATESTRUCT*)lParam)->lpCreateParams;
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)app);
			initialized = true;
		}
	}
	if (initialized)
		return ((Application*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WindowProc(hWnd, Msg, wParam, lParam);
	else
		return DefWindowProc(hWnd, Msg, wParam, lParam);
}

LRESULT CALLBACK Application::WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
		case WM_KEYDOWN:
		{
			keyboard.OnKeyPress((unsigned int)wParam);
			break;
		}

		case WM_KEYUP:
		{
			keyboard.OnKeyRelease((unsigned int)wParam);
			break;
		}

		case WM_LBUTTONDOWN:
		{
			POINT p;
			p.x = GET_X_LPARAM(lParam);
			p.y = GET_Y_LPARAM(lParam);
			mouse.OnMouseLeftDown((int)p.x, (int)p.y);
			break;
		}

		case WM_LBUTTONUP:
		{
			POINT p;
			p.x = GET_X_LPARAM(lParam);
			p.y = GET_Y_LPARAM(lParam);
			mouse.OnMouseLeftUp((int)p.x, (int)p.y);
			break;
		}

		case WM_MBUTTONDOWN:
		{
			POINT p;
			p.x = GET_X_LPARAM(lParam);
			p.y = GET_Y_LPARAM(lParam);
			mouse.OnMouseMiddleDown((int)p.x, (int)p.y);
			break;
		}

		case WM_MBUTTONUP:
		{
			POINT p;
			p.x = GET_X_LPARAM(lParam);
			p.y = GET_Y_LPARAM(lParam);
			mouse.OnMouseMiddleUp((int)p.x, (int)p.y);
			break;
		}

		case WM_RBUTTONDOWN:
		{
			POINT p;
			p.x = GET_X_LPARAM(lParam);
			p.y = GET_Y_LPARAM(lParam);
			mouse.OnMouseRightDown((int)p.x, (int)p.y);
			break;
		}

		case WM_RBUTTONUP:
		{
			POINT p;
			p.x = GET_X_LPARAM(lParam);
			p.y = GET_Y_LPARAM(lParam);
			mouse.OnMouseRightUp((int)p.x, (int)p.y);
			break;
		}

		case WM_MOUSEMOVE:
		{
			POINT p;
			p.x = GET_X_LPARAM(lParam);
			p.y = GET_Y_LPARAM(lParam);
			mouse.OnMouseMove((int)p.x, (int)p.y);
			break;
		}

		case WM_MOUSELEAVE:
		{
			mouse.OnMouseLeave();
			break;
		}

		case WM_CLOSE:
		{
			Destroy();
			break;
		}

		default:
			return DefWindowProc(hWnd, Msg, wParam, lParam);
	}
	return 0;
}

Application::Application(int x, int y, unsigned int width, unsigned int height, const wchar_t* title):
	x(x),
	y(y),
	width(width),
	height(height),
	title(title),
	running(false),
	main_window_handle(NULL)
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

	main_window_handle = CreateWindow
	(
		wc.lpszClassName,
		title,
		window_style,
		(x != -1) ? x : CW_USEDEFAULT,
		(y != -1) ? y : CW_USEDEFAULT,
		(width != 0) ? (rc.right - rc.left) : CW_USEDEFAULT,
		(height != 0) ? (rc.bottom - rc.top) : CW_USEDEFAULT,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		(void*)this
	);
}

Application::~Application()
{}

bool Application::IsKeyPressed(KeyCode code) const
{
	return keyboard.IsKeyPressed(code);
}

bool Application::IsKeyPressed(unsigned int key) const
{
	return keyboard.IsKeyPressed(key);
}

bool Application::IsMouseInWindow() const
{
	return mouse.IsInWindow();
}

bool Application::IsMouseLeftDown() const
{
	return mouse.IsLeftDown();
}

bool Application::IsMouseMiddleDown() const
{
	return mouse.IsMiddleDown();
}

bool Application::IsMouseRightDown() const
{
	return mouse.IsRightDown();
}

void Application::Run()
{
	Create();
	while (running)
	{
		Update();
		Render();
	}
}

void Application::Kill()
{
	Destroy();
}

void Application::SetPos(int x, int y)
{
	this->x = x;
	this->y = y;

	SetWindowPos(main_window_handle, HWND_TOP, x, y, width, height, SWP_NOSIZE | SWP_NOZORDER);
}

void Application::SetSize(unsigned int width, unsigned int height)
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

	SetWindowPos(main_window_handle, HWND_TOP, x, y, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER);
}

void Application::Create()
{
	OnCreate();
	running = true;
}

void Application::Update()
{
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	OnUpdate();
}

void Application::Render()
{
	RECT rc = {};
	rc.left = 0;
	rc.top = 0;
	rc.right = width;
	rc.bottom = height;
	InvalidateRect(main_window_handle, &rc, TRUE);
	PAINTSTRUCT ps;
	BeginPaint(main_window_handle, &ps);
	OnRender();
	EndPaint(main_window_handle, &ps);
}

void Application::Destroy()
{
	OnDestroy();
	PostQuitMessage(0);
	UnregisterClass(wc.lpszClassName, NULL);
	DestroyWindow(main_window_handle);
	running = false;
}