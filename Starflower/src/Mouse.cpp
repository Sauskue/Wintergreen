#include "pch.h"
#include "Application.h"

void Mouse::OnMouseLeftUp(int x, int y)
{
	left_pressed = false;
	Event::Push(Event(Event::Type::MouseLeftRelease, x, y));
}

void Mouse::OnMouseLeftDown(int x, int y)
{
	left_pressed = true;
	Event::Push(Event(Event::Type::MouseLeftPress, x, y));
}

void Mouse::OnMouseMiddleUp(int x, int y)
{
	middle_pressed = false;
	Event::Push(Event(Event::Type::MouseMiddleRelease, x, y));
}

void Mouse::OnMouseMiddleDown(int x, int y)
{
	middle_pressed = true;
	Event::Push(Event(Event::Type::MouseMiddlePress, x, y));
}

void Mouse::OnMouseRightUp(int x, int y)
{
	right_pressed = false;
	Event::Push(Event(Event::Type::MouseRightRelease, x, y));
}

void Mouse::OnMouseRightDown(int x, int y)
{
	right_pressed = true;
	Event::Push(Event(Event::Type::MouseRightPress, x, y));
}

void Mouse::OnMouseMove(int x, int y)
{
	Event::Push(Event(Event::Type::MouseMove, x, y));
}

void Mouse::OnMouseRawMove(int x, int y)
{
	Event::Push(Event(Event::Type::MouseRawMove, x, y));
}

void Mouse::OnMouseEnter()
{
	Event::Push(Event(Event::Type::MouseEnter));
}

void Mouse::OnMouseLeave()
{
	Event::Push(Event(Event::Type::MouseLeave));
}

void Mouse::OnMouseWheel(int x, int y, int delta)
{
	z_delta += delta;
	while (z_delta >= wheel_delta)
	{
		z_delta -= wheel_delta;
		OnMouseWheelUp(x, y);
	}

	while (z_delta <= -wheel_delta)
	{
		z_delta += wheel_delta;
		OnMouseWheelDown(x, y);
	}
}

void Mouse::OnMouseWheelUp(int x, int y)
{
	Event::Push(Event(Event::Type::MouseWheelUp, x, y));
}

void Mouse::OnMouseWheelDown(int x, int y)
{
	Event::Push(Event(Event::Type::MouseWheelDown, x, y));
}

bool Mouse::IsInWindow() const
{
	return in_window;
}

bool Mouse::IsLeftDown() const
{
	return left_pressed;
}

bool Mouse::IsMiddleDown() const
{
	return middle_pressed;
}

bool Mouse::IsRightDown() const
{
	return right_pressed;
}