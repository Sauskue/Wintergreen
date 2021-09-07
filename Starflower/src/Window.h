#pragma once

class Window
{
public:
	//variables
	void Init();

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

	static WNDCLASS wc;
	static bool registered;
	//methods
	static LRESULT CALLBACK StaticWindowCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK WindowCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void Update();
	void Draw();

	virtual void OnCreate() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnDraw() = 0;
	virtual void OnDestroy() = 0;
};