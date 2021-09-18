#pragma once

class Application
{
public:
	enum class KeyCode
	{
		NumLock = VK_NUMLOCK,
		Plus = VK_OEM_PLUS,
		Minus = VK_OEM_MINUS,
		Multiply = VK_OEM_MINUS,
		Divide = VK_DIVIDE,
		Equal = VK_RETURN,
		Home = VK_HOME,
		End = VK_END,
		Insert = VK_INSERT,
		Delete = VK_DELETE,
		PageUp = VK_PRIOR,
		PageDown = VK_NEXT,
		Escape = VK_ESCAPE,
		F1 = VK_F1,
		F2 = VK_F2,
		F3 = VK_F3,
		F4 = VK_F4,
		F5 = VK_F5,
		F6 = VK_F6,
		F7 = VK_F7,
		F8 = VK_F8,
		F9 = VK_F9,
		F10 = VK_F10,
		F11 = VK_F11,
		F12 = VK_F12,
		PrintScreen = VK_SNAPSHOT,
		ScrollLock = VK_SCROLL,
		Pause = VK_PAUSE,
		Break = VK_CANCEL,
		Up = VK_UP,
		Down = VK_DOWN,
		Left = VK_LEFT,
		Right = VK_RIGHT,
		Tab = VK_TAB,
		CapsLock = VK_CAPITAL,
		Shift = VK_SHIFT,
		Control = VK_CONTROL,
		Alt = VK_MENU,
		Enter = VK_RETURN,
		Space = VK_SPACE,
		BackSpace = VK_BACK,
		Comma = VK_OEM_COMMA,
		Period = VK_OEM_PERIOD,
		NumPadZero = VK_NUMPAD0,
		NumPadOne = VK_NUMPAD1,
		NumPadTwo = VK_NUMPAD2,
		NumPadThree = VK_NUMPAD3,
		NumPadFour = VK_NUMPAD4,
		NumPadFive = VK_NUMPAD5,
		NumPadSix = VK_NUMPAD6,
		NumPadSeven = VK_NUMPAD7,
		NumPadEight = VK_NUMPAD8,
		NumPadNine = VK_NUMPAD9,
		A = (char)'A',
		B = (char)'B',
		C = (char)'C',
		D = (char)'D',
		E = (char)'E',
		F = (char)'F',
		G = (char)'G',
		H = (char)'H',
		I = (char)'I',
		J = (char)'J',
		K = (char)'K',
		L = (char)'L',
		M = (char)'M',
		N = (char)'N',
		O = (char)'O',
		P = (char)'P',
		Q = (char)'Q',
		R = (char)'R',
		S = (char)'S',
		T = (char)'T',
		U = (char)'U',
		V = (char)'V',
		W = (char)'W',
		X = (char)'X',
		Y = (char)'Y',
		Z = (char)'Z',
		Zero = (char)'0',
		One = (char)'1',
		Two = (char)'2',
		Three = (char)'3',
		Four = (char)'4',
		Five = (char)'5',
		Six = (char)'6',
		Seven = (char)'7',
		Eight = (char)'8',
		Nine = (char)'9',
		//Keyboard-specific VK's//
		OpenBracket,
		CloseBracket,
		BackSlash,
		SemiColon,
		Apostrophe,
	};
private:
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
		int GetX() const;
		int GetY() const;
		unsigned int GetWidth() const;
		unsigned int GetHeight() const;

		void SetPos(int x, int y);
		void SetSize(unsigned int width, unsigned int height);

		static LRESULT CALLBACK StaticWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
		LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	};

	class Keyboard
	{
	public:
		//classes

		//variables

		//methods
		void OnKeyPress(KeyCode code);
		void OnKeyPress(unsigned int key);
		void OnKeyRelease(KeyCode code);
		void OnKeyRelease(unsigned int key);

		bool IsKeyPressed(KeyCode code) const;
		bool IsKeyPressed(unsigned int key) const;

	protected:
	private:
		//variables

		//methods
	};

	class Mouse
	{
	public:
		//variables

		//methods
		void OnLeftButtonDown(int x, int y);
		void OnLeftButtonUp(int x, int y);
		void OnMiddleButtonDown(int x, int y);
		void OnMiddleButtonUp(int x, int y);
		void OnRightButtonDown(int x, int y);
		void OnRightButtonUp(int x, int y);
		void OnMouseMove(int x, int y);
		void OnRawMouseMove(int x, int y);
		void OnMouseEnter();
		void OnMouseLeave();
		void OnMouseWheelUp(int x, int y);
		void OnMouseWheelDown(int x, int y);
		void OnMouseWheel(int x, int y, int delta);

		bool IsInWindow() const;
		bool IsLeftDown() const;
		bool IsMiddleDown() const;
		bool IsRightDown() const;
	protected:
	private:
		//variables
	};

	//variables
	bool running = false;

	Window* main_window = nullptr;
	Keyboard keyboard;
	Mouse mouse;

	//methods
	void Create();
	void Update();
	void Render();

	virtual void OnCreate() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnRender() = 0;
	virtual void OnDestroy() = 0;
protected:
	//classes

	//variables

	//methods
	Application();


	/*TEST CODE*/
	HWND GetMainWindowHandle() const;

	bool IsKeyPressed(KeyCode code) const;
	bool IsKeyPressed(unsigned int key) const;
	bool IsMouseInWindow() const;
	bool IsMouseLeftDown() const;
	bool IsMouseMiddleDown() const;
	bool IsMouseRightDown() const;

	/////////////
public:
	//classes
	class Event
	{
	public:
	protected:
	private:
	};

	//variables

	//methods
	void Run();
	void Kill();
	void SetPos(int x, int y);
	void SetSize(unsigned int width, unsigned int height);
	virtual ~Application();

};


#define KeyCode Application::KeyCode