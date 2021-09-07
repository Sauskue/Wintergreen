#include "pch.h"
#include "Demos.h"

static const int width = 800;
static const int height = 600;
static HDC win_dc = NULL;
static HDC mem_dc = NULL;
static HBITMAP bmp = NULL;

static bool cells[height][width] = { 0 };
static bool cells_next[height][width] = { 0 };
static const int scale = 1;
static unsigned char* bits = nullptr;

/*KEY PRINCIPLES
1)DEATH-STATE CHANGED FROM 1 TO 0
	1.1)DEATH FROM OVERPOPULATION(TOO CROWDED) IF > 3 NEIGHBORS ARE ALIVE
	1.2)LONELINESS IF < 2 NEIGHBORS ARE ALIVE
2)BIRTH-STATE CHANGED FROM 0 TO 1
	2.1)ONLY HAPPENS IF EXACTLY 3 LIVE NEIGHBORS
3)STASIS
	3.1)IN ALL OTHER CASES STATE REMAINS SAME
*/

void Update()
{
	//check for neighbors
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			int neighbors = 0;
			for(int n_row = row - 1; n_row < row + 2; n_row++)
			{
				if (n_row < 0 || n_row >= height)
					continue;
				for (int n_col = col - 1; n_col < col + 2; n_col++)
				{
					if (n_col < 0 || n_col >= width || (n_row == row && n_col == col))
						continue;
					if (cells[n_row][n_col])
						neighbors++;
				}
			}
			if (cells[row][col])
			{
				if (neighbors == 2 || neighbors == 3)
					cells_next[row][col] = 1;//survives
				else if (neighbors >= 4)
					cells_next[row][col] = 0;//dies(overpopulation)
				else if (neighbors <= 1)
					cells_next[row][col] = 0;//dies(loneliness)
			}
			else
			{
				if (neighbors == 3)
					cells_next[row][col] = 1;//birth
				else
					cells_next[row][col] = cells[row][col];//stasis
			}
		}
	}

	//then update cells
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			cells[row][col] = cells_next[row][col];
			bits[(row * 4 * width) + (4 * col) + 0] = cells[row][col] * 255; //blue
			bits[(row * 4 * width) + (4 * col) + 1] = cells[row][col] * 255; //green
			bits[(row * 4 * width) + (4 * col) + 2] = cells[row][col] * 255; //red
			bits[(row * 4 * width) + (4 * col) + 3] = (unsigned char)0;		 //reserved(set to 0)
		}
	}
}

LRESULT CALLBACK GameOfLifeCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_CREATE:
		{
			win_dc = GetDC(hwnd);
			mem_dc = CreateCompatibleDC(win_dc);

			RECT rc;
			GetClientRect(hwnd, &rc);

			BITMAPINFO bmi;
			ZeroMemory(&bmi, sizeof(BITMAPINFO));
			bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bmi.bmiHeader.biWidth = rc.right - rc.left;
			bmi.bmiHeader.biHeight = rc.top - rc.bottom;
			bmi.bmiHeader.biPlanes = 1;
			bmi.bmiHeader.biBitCount = 32;

			bmp = CreateDIBSection(mem_dc, &bmi, DIB_RGB_COLORS, (void**)&bits, NULL, NULL);
			HGDIOBJ old = SelectObject(mem_dc, bmp);

			BitBlt
			(
				win_dc,
				0,
				0,
				rc.right - rc.left,
				rc.bottom - rc.top,
				mem_dc,
				0,
				0,
				SRCCOPY
			);
			SelectObject(win_dc, old);

			std::srand((unsigned int)std::time(0));
			for (int row = 0; row < height; row++)
			{
				for (int col = 0; col < width; col++)
				{
					cells[row][col] = std::rand() % 2;
					cells_next[row][col] = cells[row][col];
					bits[(row * 4 * width) + (4 * col) + 0] = cells[row][col] * 255; //blue
					bits[(row * 4 * width) + (4 * col) + 1] = cells[row][col] * 255; //green
					bits[(row * 4 * width) + (4 * col) + 2] = cells[row][col] * 255; //red
					bits[(row * 4 * width) + (4 * col) + 3] = (unsigned char)0;		 //reserved(set to 0)
				}
			}

			BitBlt
			(
				win_dc,
				0,
				0,
				rc.right - rc.left,
				rc.bottom - rc.top,
				mem_dc,
				0,
				0,
				SRCCOPY
			);
			SelectObject(win_dc, old);
			break;
		}
		case WM_PAINT:
		{
			Update();
			PAINTSTRUCT ps;
			BeginPaint(hwnd, &ps);
			HGDIOBJ old = SelectObject(mem_dc, bmp);
			BitBlt
			(
				win_dc,
				0,
				0,
				width,
				height,
				mem_dc,
				0,
				0,
				SRCCOPY
			);
			SelectObject(win_dc, old);
			EndPaint(hwnd, &ps);

			break;
		}

		case WM_CLOSE:
			PostQuitMessage(0);
			break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
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
	HWND hwnd = CreateWindow
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
				DestroyWindow(hwnd);
				UnregisterClass(wc.lpszClassName, NULL);
				return (int)msg.wParam;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		InvalidateRect(hwnd, &rc, TRUE);
	}
}