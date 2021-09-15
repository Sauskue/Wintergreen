#include "pch.h"
#include "Application.h"

Application::Application()
{
	main_window = new Window();
	running = true;
}

Application::~Application()
{}

/*TEST CODE*/
HWND Application::GetMainWindowHandle() const
{
	return main_window->GetWindowHandle();
}
/////////////

void Application::Run()
{
	OnCreate();
	while (running)
	{
		Update();
		if (running)
			Render();
	}
}

void Application::Kill()
{
	OnDestroy();
	running = false;
}

void Application::Update()
{
	OnUpdate();
	if (running)
		if (!Window::Update())
			running = false;
}

void Application::Render()
{
	HWND hwnd = main_window->GetWindowHandle();
	RECT rc = {};
	rc.left = 0;
	rc.top = 0;
	rc.right = main_window->GetWidth();
	rc.bottom = main_window->GetHeight();
	InvalidateRect(hwnd, &rc, TRUE);
	PAINTSTRUCT ps;
	BeginPaint(hwnd, &ps);
	OnRender();
	EndPaint(hwnd, &ps);
}