#pragma once
#include "Application.h"

WG::Application* CreateApp();

int main()
{
	auto app = WG::CreateApp();
	return app->Run();
	delete app;
}