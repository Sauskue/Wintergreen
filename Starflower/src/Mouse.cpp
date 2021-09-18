#include "pch.h"
#include "Application.h"

static std::queue<Application::Event> buffer;
static const unsigned int max_buffer_size = 32u;
static const int wheel_delta = 120;
static bool left_pressed = false;
static bool middle_pressed = false;
static bool right_pressed = false;
static bool in_window = false;
static int x = 0;
static int y = 0;
static int z_delta = 0;

void Application::Mouse::OnLeftButtonDown(int x, int y)
{
	left_pressed = true;
}

void Application::Mouse::OnLeftButtonUp(int x, int y)
{}

void Application::Mouse::OnMiddleButtonDown(int x, int y)
{}

void Application::Mouse::OnMiddleButtonUp(int x, int y)
{}

void Application::Mouse::OnRightButtonDown(int x, int y)
{}

void Application::Mouse::OnRightButtonUp(int x, int y)
{}

void Application::Mouse::OnMouseMove(int x, int y)
{}

void Application::Mouse::OnRawMouseMove(int x, int y)
{}

void Application::Mouse::OnMouseEnter()
{}

void Application::Mouse::OnMouseLeave()
{}

void Application::Mouse::OnMouseWheelUp(int x, int y)
{}

void Application::Mouse::OnMouseWheelDown(int x, int y)
{}

void Application::Mouse::OnMouseWheel(int x, int y, int delta)
{}

bool Application::Mouse::IsInWindow() const
{
	return in_window;
}

bool Application::Mouse::IsLeftDown() const
{
	return left_pressed;
}

bool Application::Mouse::IsMiddleDown() const
{
	return middle_pressed;
}

bool Application::Mouse::IsRightDown() const
{
	return right_pressed;
}