#include "pch.h"
#include "Demos.h"

static int width = 800;
static int height = 600;

LRESULT CALLBACK GameOfLifeCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_PAINT:
		{
			;
		}

		case WM_CLOSE:
			PostQuitMessage(0);
			break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

int GameOfLife()
{
	WNDCLASS wc =
	{
		CS_OWNDC | CS_HREDRAW | CS_VREDRAW,
		GameOfLifeCallback,
		0,
		0,
		GetModuleHandle(NULL),
		NULL,
		NULL,
		NULL,
		NULL,
		L"Game of Life Window Class"
	};
	RegisterClass(&wc);
	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = width;
	rc.bottom = height;
	AdjustWindowRect(&rc, (WS_OVERLAPPEDWINDOW | WS_VISIBLE) & (~WS_THICKFRAME), FALSE);
	HWND hWnd = CreateWindow
	(
		wc.lpszClassName,
		L"Game of Life Demo",
		(WS_OVERLAPPEDWINDOW | WS_VISIBLE) & (~WS_THICKFRAME),
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		NULL

	);
	while (true)
	{
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			if (msg.message == WM_QUIT)
			{
				DestroyWindow(hWnd);
				UnregisterClass(wc.lpszClassName, NULL);
				return (int)msg.wParam;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		UpdateWindow(hWnd);
	}
}