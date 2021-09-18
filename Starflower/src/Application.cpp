#include "pch.h"
#include "Application.h"

Application::Application()
{}

Application::~Application()
{}

/*TEST CODE*/
HWND Application::GetMainWindowHandle() const
{
	return main_window->GetWindowHandle();
}

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
/////////////

void Application::Run()
{
	Create();
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
	delete main_window;
	running = false;
}

void Application::SetPos(int x, int y)
{
	main_window->SetPos(x, y);
}

void Application::SetSize(unsigned int width, unsigned int height)
{
	main_window->SetSize(width, height);
}

void Application::Create()
{
	main_window = new Window();
	running = true;
	OnCreate();
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