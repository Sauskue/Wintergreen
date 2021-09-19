#include "pch.h"
#include "Application.h"

std::queue<Application::Event> buffer;
const unsigned int max_buffer_size = 32u;

Application::Event::Event()
	:x(0), y(0), key(0u), type(Type::Invalid)
{}

Application::Event::Event(Event::Type type)
	: x(0), y(0), key(0u), type(type)
{}

Application::Event::Event(Event::Type type, int x, int y)
	: x(x), y(y), key(0u), type(type)
{}

Application::Event::Event(Event::Type type, unsigned int key)
	: x(0), y(0), key(key), type(type)
{}

bool Application::IsBufferEmpty() const
{
	return !(buffer.size() > 0);
}

void Application::TrimBuffer()
{
	while (buffer.size() >= max_buffer_size)
		buffer.pop();
}

void Application::FlushBuffer()
{
	buffer = std::queue<Application::Event>();
}

Application::Event Application::GetEvent()
{
	if (buffer.size() > 0)
	{
		Application::Event e = buffer.front();
		buffer.pop();
		return e;
	}
	return Application::Event();
}

int Application::Event::GetX() const
{
	return x;
}

int Application::Event::GetY() const
{
	return y;
}

Application::Event::Type Application::Event::GetType() const
{
	return type;
}

KeyCode Application::Event::GetKeyCode() const
{
	return (KeyCode)key;
}