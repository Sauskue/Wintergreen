#include "pch.h"
#include "Demos.h"

/*KEY PRINCIPLES
1)DEATH-STATE CHANGED FROM 1 TO 0
	1.1)DEATH FROM OVERPOPULATION(TOO CROWDED) IF > 3 NEIGHBORS ARE ALIVE
	1.2)LONELINESS IF < 2 NEIGHBORS ARE ALIVE
2)BIRTH-STATE CHANGED FROM 0 TO 1
	2.1)ONLY HAPPENS IF EXACTLY 3 LIVE NEIGHBORS
3)STASIS
	3.1)IN ALL OTHER CASES STATE REMAINS SAME
*/
const int GameOfLife::width;
const int GameOfLife::height;

void GameOfLife::OnCreate()
{
	SetSize(width, height);
	win_dc = GetDC(main_window_handle);
	mem_dc = CreateCompatibleDC(win_dc);

	RECT rc;
	GetClientRect(main_window_handle, &rc);

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
}

void GameOfLife::OnUpdate()
{
	//check for neighbors
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			int neighbors = 0;
			for (int n_row = row - 1; n_row < row + 2; n_row++)
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

void GameOfLife::OnRender()
{
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
}

void GameOfLife::OnDestroy()
{

}