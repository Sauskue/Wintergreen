#pragma once

class Application
{
public:
	//variables
	
	//methods
	void Run();
	void Kill();
	virtual ~Application();
protected:
	//classes
	class Window
	{
		friend class Application;
	public:
		//variables

		//methods

	protected:
		//variables

		//methods
		
	private:
		//variables
		int x = 0;
		int y = 0;
		int width = 0;
		int height = 0;
		int index = 0;
		const wchar_t* title = L"Window";
		HWND window_handle = NULL;
		bool alive = false;

		static WNDCLASS wc;
		static bool registered;
		static std::vector<Window*> windows;

		//methods
		Window();
		Window(int x, int y);
		Window(int x, int y, int width, int height);
		~Window();

		static bool Update();

		void SetPos(int x, int y);
		void SetSize(int width, int height);

		static LRESULT CALLBACK StaticWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
		LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	};
	
	//variables

	//methods
	Application();
private:
	//variables
	bool running = false;

	Window* main_window = nullptr;

	//methods
	void Update();
	void Render();

	virtual void OnCreate() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnRender() = 0;
	virtual void OnDestroy() = 0;
};