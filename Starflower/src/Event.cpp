#include "pch.h"
#include "Application.h"

static std::queue<Event> buffer;
static const unsigned int max_buffer_size = 32u;

Event::Event()
	:x(0), y(0), key(0u), type(Type::Invalid)
{}

Event::Event(Event::Type type)
	: x(0), y(0), key(0u), type(type)
{}

Event::Event(Event::Type type, int x, int y)
	: x(x), y(y), key(0u), type(type)
{}

Event::Event(Event::Type type, unsigned int key)
	: x(0), y(0), key(key), type(type)
{}

bool Event::IsBufferEmpty()
{
	return !(buffer.size() > 0);
}

void Event::Push(Event e)
{
	buffer.push(e);
	TrimBuffer();
}

Event Event::ReadBuffer()
{
	if (buffer.size() > 0)
	{
		Event e = buffer.front();
		buffer.pop();
		return e;
	}
	return Event();
}

void Event::TrimBuffer()
{
	while (buffer.size() >= max_buffer_size)
		buffer.pop();
}

void Event::FlushBuffer()
{
	buffer = std::queue<Event>();
}

int Event::GetX() const
{
	return x;
}

int Event::GetY() const
{
	return y;
}

Event::Type Event::GetType() const
{
	return type;
}

KeyCode Event::GetKeyCode() const
{
	return (KeyCode)key;
}