#include "pch.h"
#include "Application.h"

Application::Application()
{
	main_window = new Window();
	running = true;
}

Application::~Application()
{}

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
	OnRender();
}