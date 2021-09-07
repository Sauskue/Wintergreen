#pragma once

class Window
{
public:
	//variables
	static void ProcessMessages();

	//methods
	virtual ~Window();
protected:
	//variables

	//methods
	Window();
private:
	//variables
	unsigned int x;
	unsigned int y;
	unsigned int width;
	unsigned int height;
	const wchar_t* title;
	HWND window_handle;
	bool alive;

	static WNDCLASS wc;
	static bool registered;
	static std::vector<Window*> windows;
	//methods
	static LRESULT CALLBACK StaticWindowCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK WindowCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual void OnCreate() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnDraw() = 0;
	virtual void OnDestroy() = 0;
};