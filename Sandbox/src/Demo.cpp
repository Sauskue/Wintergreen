#include "Wintergreen.h"

class Sandbox : public WG::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

WG::Application* WG::CreateApp()
{
	return new Sandbox();
}