#pragma once

namespace WG
{
	class Application
	{
	public:
		Application
		();
		virtual ~Application();

		int Run();

	private:
		bool InitCOM();
		bool InitWindow();

		//std::optional<int> CheckMessages();

	private:
	};

	Application* CreateApp();
}