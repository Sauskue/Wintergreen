#pragma once

class Keyboard
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
		Backspace = VK_BACK,
		Comma = VK_OEM_COMMA,
		Period = VK_OEM_PERIOD,
		NumpadZero = VK_NUMPAD0,
		NumpadOne = VK_NUMPAD1,
		NumpadTwo = VK_NUMPAD2,
		NumpadThree = VK_NUMPAD3,
		NumpadFour = VK_NUMPAD4,
		NumpadFive = VK_NUMPAD5,
		NumpadSix = VK_NUMPAD6,
		NumpadSeven = VK_NUMPAD7,
		NumpadEight = VK_NUMPAD8,
		NumpadNine = VK_NUMPAD9,
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
		Backslash,
		Semicolon,
		Apostrophe,
	};
	
	void OnKeyPress(KeyCode code);
	void OnKeyPress(unsigned int key);
	void OnKeyRelease(KeyCode code);
	void OnKeyRelease(unsigned int key);

	bool IsKeyPressed(KeyCode code) const;
	bool IsKeyPressed(unsigned int key) const;
protected:
private:
	std::bitset<254> keys;
};

class Mouse
{
public:
	void OnMouseLeftUp(int x, int y);
	void OnMouseLeftDown(int x, int y);
	void OnMouseMiddleUp(int x, int y);
	void OnMouseMiddleDown(int x, int y);
	void OnMouseRightUp(int x, int y);
	void OnMouseRightDown(int x, int y);
	void OnMouseMove(int x, int y);
	void OnMouseRawMove(int x, int y);
	void OnMouseEnter();
	void OnMouseLeave();
	void OnMouseWheel(int x, int y, int delta);
	void OnMouseWheelUp(int x, int y);
	void OnMouseWheelDown(int x, int y);

	bool IsInWindow() const;
	bool IsLeftDown() const;
	bool IsMiddleDown() const;
	bool IsRightDown() const;
	int GetX() const;
	int GetY() const;
protected:
private:
	const int wheel_delta = 120;
	bool left_pressed = false;
	bool middle_pressed = false;
	bool right_pressed = false;
	bool in_window = false;
	int x = 0;
	int y = 0;
	int z_delta = 0;

};

class Event
{
public:
	enum class Type
		{
			KeyPress,
			KeyRelease,
			MouseLeftPress,
			MouseLeftRelease,
			MouseMiddlePress,
			MouseMiddleRelease,
			MouseRightPress,
			MouseRightRelease,
			MouseWheelUp,
			MouseWheelDown,
			MouseMove,
			MouseRawMove,
			MouseEnter,
			MouseLeave,
			Invalid
		};

	Event();
	Event(Type type);
	Event(Type type, int x, int y);
	Event(Type type, unsigned int key);

	static void FlushBuffer();
	static bool IsBufferEmpty();
	static void Push(Event e);

	static Event ReadBuffer();

	int GetX() const;
	int GetY() const;
	Type GetType() const;
	Keyboard::KeyCode GetKeyCode() const;
protected:
private:
	int x;
	int y;
	unsigned int key;
	Type type;
	
	static void TrimBuffer();
};

class Application
{
public:
	//methods
	void Run();
	void Kill();
	virtual ~Application();

protected:
	//variables
	HWND main_window_handle;
	
	//methods
	Application(int x = -1, int y = -1, unsigned int width = 0, unsigned int height = 0, const wchar_t* title = L"Window");

	bool IsKeyPressed(Keyboard::KeyCode code) const;
	bool IsKeyPressed(unsigned int key) const;
	bool IsMouseInWindow() const;
	bool IsMouseLeftDown() const;
	bool IsMouseMiddleDown() const;
	bool IsMouseRightDown() const;
	bool IsEventBufferEmpty() const;
	
	int GetMouseX() const;
	int GetMouseY() const;

	void SetPos(int x, int y);
	void SetSize(unsigned int width, unsigned int height);
	void SetTitle(const wchar_t* title);

	Event ReadEventBuffer();

private:
	//variables
	int x;
	int y;
	unsigned int width;
	unsigned int height;
	const wchar_t* title;
	bool running;

	Keyboard keyboard;
	Mouse mouse;

	static bool registered;

	//methods
	void Create();
	void Update();
	void Render();
	void Destroy();

	virtual void OnCreate() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnRender() = 0;
	virtual void OnDestroy() = 0;

	static LRESULT CALLBACK StaticWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
};

#define KeyCode Keyboard::KeyCode