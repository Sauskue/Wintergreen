#include "pch.h"
#include "Application.h"

void Mouse::OnMouseLeftUp(int x, int y)
{
	left_pressed = false;
}

void Mouse::OnMouseLeftDown(int x, int y)
{
	left_pressed = true;
}

void Mouse::OnMouseMiddleUp(int x, int y)
{
	middle_pressed = false;
}

void Mouse::OnMouseMiddleDown(int x, int y)
{
	middle_pressed = true;
}

void Mouse::OnMouseRightUp(int x, int y)
{
	right_pressed = false;
}

void Mouse::OnMouseRightDown(int x, int y)
{
	right_pressed = true;
}

void Mouse::OnMouseMove(int x, int y)
{}

void Mouse::OnMouseRawMove(int x, int y)
{}

void Mouse::OnMouseEnter()
{}

void Mouse::OnMouseLeave()
{}

void Mouse::OnMouseWheel(int x, int y, int delta)
{}

void Mouse::OnMouseWheelUp(int x, int y)
{}

void Mouse::OnMouseWheelDown(int x, int y)
{}

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