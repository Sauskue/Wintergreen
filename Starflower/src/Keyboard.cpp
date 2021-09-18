#include "pch.h"
#include "Application.h"

static std::queue<Application::Event> buffer;
static std::bitset<254> keys;
static const unsigned int max_buffer_size = 32u;

void Application::Keyboard::OnKeyPress(KeyCode code)
{
	this->OnKeyPress((unsigned int)code);
}

void Application::Keyboard::OnKeyPress(unsigned int key)
{
	keys[key] = 1;
}

void Application::Keyboard::OnKeyRelease(KeyCode code)
{
	this->OnKeyRelease((unsigned int)code);
}

void Application::Keyboard::OnKeyRelease(unsigned int key)
{
	keys[key] = 0;
}

bool Application::Keyboard::IsKeyPressed(KeyCode code) const
{
	return this->IsKeyPressed((unsigned int)code);
}

bool Application::Keyboard::IsKeyPressed(unsigned int key) const
{
	return keys[key];
}