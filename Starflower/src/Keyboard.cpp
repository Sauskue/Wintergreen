#include "pch.h"
#include "Application.h"

void Keyboard::OnKeyPress(KeyCode code)
{
	OnKeyPress((unsigned int)code);
}

void Keyboard::OnKeyPress(unsigned int key)
{
	keys[key] = 1;
	buffer.push(Event(Event::Type::KeyPress, key));
	TrimBuffer();
}

void Keyboard::OnKeyRelease(KeyCode code)
{
	OnKeyRelease((unsigned int)code);
}

void Keyboard::OnKeyRelease(unsigned int key)
{
	keys[key] = 0;
}

bool Keyboard::IsKeyPressed(KeyCode code) const
{
	return IsKeyPressed((unsigned int)code);
}

bool Keyboard::IsKeyPressed(unsigned int key) const
{
	return keys[key];
}