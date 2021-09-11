#include "pch.h"
#include "Demos.h"
#include "Application.h"
//(TODO): add keyboard/mouse events
//(TODO): put demo applications in demo.h
//(TODO): window-specific events???
//(TODO): does main_window need to be manually deleted or is it handled by destructor??
//(TODO): work on main update-close loop
//TEST CODE
class PerlinN : public Application
{
private:
	void OnCreate() override{}

	void OnDestroy() override {}
	void OnUpdate() override
	{
		std::cout << "Update called\n";
	}

	void OnRender() override
	{
		std::cout << "Render called\n";
	}
};

int main()
{
	PerlinN demo;
	demo.Run();
}