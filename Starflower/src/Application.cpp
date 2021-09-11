#include "pch.h"
#include "Application.h"

Application::Application()
{
	main_window = new Window();
	running = true;
}

Application::~Application()
{
	running = false;
}

void Application::Run()
{
	while (running)
	{
		if (Update())
			Render();
		else
			this->~Application();
	}
}

void Application::Kill()
{
	this->~Application();
}

bool Application::Update()
{
	OnUpdate();
	if (!Window::Update() || !running)
		return false;
	else
		return true;
}

void Application::Render()
{
	OnRender();
}