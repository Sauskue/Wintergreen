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
		//classes

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
	
		static std::vector<Window*> windows;
		static WNDCLASS wc;
		static bool registered;
		
		//methods
		Window();
		Window(int x, int y);
		Window(int x, int y, int width, int height);
		~Window();

		static bool Update();

		HWND GetWindowHandle() const;
		unsigned int GetWidth() const;
		unsigned int GetHeight() const;

		void SetPos(int x, int y);
		void SetSize(unsigned int width, unsigned int height);

		static LRESULT CALLBACK StaticWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
		LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	};

	//variables

	//methods
	Application();

	/*TEST CODE*/
	HWND GetMainWindowHandle() const;

	/////////////


	void SetPos(int x, int y);
	void SetSize(unsigned int width, unsigned int height);
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