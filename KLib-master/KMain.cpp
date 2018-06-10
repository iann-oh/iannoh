#include "stdafx.h"
#include "KMain.h"
#include "resource.h"
#include <stdio.h>
#include<time.h>
#include<stdlib.h>
#include <Windows.h>

void OnCreate(HWND hWnd, HINSTANCE hInst);
void OnDraw(HDC hdc);
void OnLButtonDown(HWND hWnd, LPARAM lParam);
void OnKeyDown(HWND hWnd, WPARAM wParam);
void OnTimer(WPARAM wParam);
int Check(int rotation, int sx, int sy);
void drawRectangle(HDC hdc, int x, int y, int w, int h, COLORREF r);

#define RECT_WIDTH 20
#define RECT_HEIGHT 20

#define SIZE_WIDTH 10
#define SIZE_HEIGHT 20

#define DIR_LEFT 0
#define DIR_RIGHT 1
#define DIR_UP 2
#define DIR_DOWN 3

#define block_O 0
#define block_I 1
#define block_Z 2
#define block_S 3
#define block_L 4
#define block_J 5
#define block_T 6

HWND mHwnd;
HINSTANCE mInst;
int rot = 1;
int block[7][4][4][4] = {
		{
			{
				0,0,0,0,
				0,1,1,0,
				0,1,1,0,
				0,0,0,0 },
				{
					0,0,0,0,
					0,1,1,0,
					0,1,1,0,
					0,0,0,0 },
					{
						0,0,0,0,
						0,1,1,0,
						0,1,1,0,
						0,0,0,0 },
						{
							0,0,0,0,
							0,1,1,0,
							0,1,1,0,
							0,0,0,0 }
		},
		{
			{
				0,0,0,0,
				1,1,1,1,
				0,0,0,0,
				0,0,0,0 },
				{
					0,0,1,0,
					0,0,1,0,
					0,0,1,0,
					0,0,1,0 },
					{
						0,0,0,0,
						1,1,1,1,
						0,0,0,0,
						0,0,0,0 },
						{
							0,0,1,0,
							0,0,1,0,
							0,0,1,0,
							0,0,1,0 }
		},
		{
			{
				0,0,0,0,
				0,1,1,0,
				0,0,1,1,
				0,0,0,0 },
				{
					0,0,0,1,
					0,0,1,1,
					0,0,1,0,
					0,0,0,0 },
					{
						0,0,0,0,
						0,1,1,0,
						0,0,1,1,
						0,0,0,0 },
						{
							0,0,0,1,
							0,0,1,1,
							0,0,1,0,
							0,0,0,0 }
		},
		{
			{
				0,0,0,0,
				0,0,1,1,
				0,1,1,0,
				0,0,0,0 },
				{
					0,0,1,0,
					0,0,1,1,
					0,0,0,1,
					0,0,0,0 },
					{
						0,0,0,0,
						0,0,1,1,
						0,1,1,0,
						0,0,0,0 },
						{
							0,0,1,0,
							0,0,1,1,
							0,0,0,1,
							0,0,0,0 }
		},
		{
			{
				0,0,0,0,
				0,1,1,1,
				0,1,0,0,
				0,0,0,0 },
				{
					0,0,1,0,
					0,0,1,0,
					0,0,1,1,
					0,0,0,0 },
					{
						0,0,0,1,
						0,1,1,1,
						0,0,0,0,
						0,0,0,0 },
						{
							0,1,1,0,
							0,0,1,0,
							0,0,1,0,
							0,0,0,0 }
		},
		{
			{
				0,0,0,0,
				0,1,1,1,
				0,0,0,1,
				0,0,0,0 },
				{
					0,0,1,1,
					0,0,1,0,
					0,0,1,0,
					0,0,0,0 },
					{
						0,1,0,0,
						0,1,1,1,
						0,0,0,0,
						0,0,0,0 },
						{
							0,0,1,0,
							0,0,1,0,
							0,1,1,0,
							0,0,0,0 }
		},
		{
			{
				0,0,0,0,
				0,1,1,1,
				0,0,1,0,
				0,0,0,0 },
				{
					0,0,1,0,
					0,0,1,1,
					0,0,1,0,
					0,0,0,0 },
					{
						0,0,1,0,
						0,1,1,1,
						0,0,0,0,
						0,0,0,0 },
						{
							0,0,1,0,
							0,1,1,0,
							0,0,1,0,
							0,0,0,0 }
		}
};

int block_shape = rand() % 7;
int next_block[3] = { rand() % 7, rand() % 7, rand() % 7 };

int map[SIZE_HEIGHT + 2][SIZE_WIDTH + 2] = { 0 };

HBITMAP hBitmap;
RECT crt;

int gx = (SIZE_WIDTH / 2) - 2, gy = -1;


// 초기화 함수
void OnCreate(HWND hWnd, HINSTANCE hInst)
{
	mHwnd = hWnd;
	mInst = hInst;
	srand(time(NULL));

	SetTimer(hWnd, 1, 300, NULL);

	for (int y = 0; y < SIZE_HEIGHT + 2; y++)
	{
		for (int x = 0; x < SIZE_WIDTH + 2; x++)
		{
			if ((x == 0) || (x == SIZE_WIDTH + 1))
			{
				map[y][x] = 1;
			}
			else if (y == SIZE_HEIGHT + 1)
			{
				map[y][x] = 2;
			}
		}
	}
}

void drawRectangle(HDC hdc, int x, int y, int w, int h, COLORREF r)
{
	HBRUSH hBrush, oBrush;
	hBrush = CreateSolidBrush(r);
	oBrush = (HBRUSH)SelectObject(hdc, hBrush);
	Rectangle(hdc, x, y, w, h);


	SelectObject(hdc, oBrush);
	DeleteObject(hBrush);
}

void OnDraw(HDC hdc)
{
	HDC hMemDC;
	HBITMAP OldBitmap;

	hMemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
	/////////////////////////////////////////////////////////////////////////////////////////

	for (int y = 0; y < SIZE_HEIGHT + 2; y++)
	{
		for (int x = 0; x < SIZE_WIDTH + 2; x++)
		{
			//Rectangle(hdc, RECT_WIDTH*x, RECT_HEIGHT*y, RECT_WIDTH*x + RECT_WIDTH, RECT_HEIGHT*y + RECT_HEIGHT);
			if ((map[y][x] == 1))
			{
				drawRectangle(hdc, RECT_WIDTH * x, RECT_HEIGHT *y, RECT_WIDTH*x + RECT_WIDTH, RECT_HEIGHT * y + RECT_HEIGHT, RGB(0, 0, 0));
			}

			if ((map[y][x] == 2))
			{
				drawRectangle(hdc, RECT_WIDTH * x, RECT_HEIGHT *y, RECT_WIDTH*(x + 1), RECT_HEIGHT * (y + 1), RGB(255, 0, 0));
			}
			if ((y == SIZE_HEIGHT + 1))
			{
				drawRectangle(hdc, RECT_WIDTH * x, RECT_HEIGHT *y, RECT_WIDTH*(x + 1), RECT_HEIGHT * (y + 1), RGB(0, 0, 0));
			}
		}
		for (int i = 0; i < 3; i++)
		{
			for (int y = 0; y < 4; y++)
			{
				for (int x = 0; x < 4; x++)
				{
					if (block[next_block[i]][0][y][x] == 1)
					{
						drawRectangle(hdc, RECT_WIDTH * (x + 20), RECT_HEIGHT *(y + 5 * i), RECT_WIDTH*(x + 21), RECT_HEIGHT * (y + 5 * i + 1), RGB(255, 0, 0));
					}
				}
			}
		}
	}

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			//Rectangle(hdc, RECT_WIDTH * x, RECT_HEIGHT * y, RECT_WIDTH * x + RECT_WIDTH, RECT_HEIGHT * y + RECT_HEIGHT);
			if (block[block_shape][rot][y][x] == 1)
			{
				drawRectangle(hdc, RECT_WIDTH * (x + gx), RECT_HEIGHT *(y + gy), RECT_WIDTH*(x + gx + 1), RECT_HEIGHT * (y + gy + 1), RGB(255, 0, 0));
			}
		}
	}


	//////////////////////////////////////////////////////////////////
	//BitBlt(hdc, x*RECT_WIDTH, y*RECT_HEIGHT, RECT_WIDTH, RECT_HEIGHT, hMemDC, RECT_WIDTH*sx,RECT_HEIGHT * sy, SRCCOPY);
	SelectObject(hMemDC, OldBitmap);
	DeleteDC(hMemDC); // hMemDC 제거

}



void OnLButtonDown(HWND hWnd, LPARAM lParam)
{

	int lx = LOWORD(lParam);
	int ly = HIWORD(lParam);
}



void OnKeyDown(HWND hWnd, WPARAM wParam)
{
	switch (wParam)
	{
	case VK_LEFT:
		if (Check(rot, gx - 1, gy) == 0)
		{
			gx--;
		}
		break;
	case VK_RIGHT:
		if (Check(rot, gx + 1, gy) == 0)
		{
			gx++;
		}
		break;
	case VK_DOWN:
		if (Check(rot, gx, gy + 1) == 0)
		{
			gy++;
		}
		break;
	case VK_UP:
		if (Check(((rot + 1) % 4), gx, gy) == 0)
		{
			rot = (rot + 1) % 4;
		}
		break;
	case VK_SPACE:
		int check_space = 0;
		int i;
		for (i = 0; i < SIZE_HEIGHT; i++)
		{
			if (Check(rot, gx, gy + i) == 0)
			{
				check_space = i;
			}
			else
			{
				break;
			}
		}
		gy += i - 1;
		break;
	}
	InvalidateRect(hWnd, NULL, true);
}



void OnTimer(WPARAM wParam)
{
	switch (wParam)
	{
	case 1:
		if (Check(rot, gx, gy + 1) == 0)
		{
			gy++;
		}
		else if (Check(rot, gx, gy + 1) == -2)
		{
			for (int y = 0; y < 4; y++)
			{
				for (int x = 0; x < 4; x++)
				{
					if (block[block_shape][rot][y][x] == 1)
					{
						map[gy + y][gx + x] = 2;

					}
				}
				//game over
				for (int y = 0; y < 4; y++)
				{
					for (int x = 0; x < 4; x++)
					{
						if (block[block_shape][rot][y][x] < -2)
						{
							exit(0);

							break;
						}
					}
				}
			}
			block_shape = next_block[0];
			next_block[0] = next_block[1];
			next_block[1] = next_block[2];
			next_block[2] = rand() % 7;
			gx = (SIZE_WIDTH / 2) - 2;
			gy = -1;
			rot = 0;

			// 줄체크
			int arr[4] = { -1 };
			int a = 0;
			for (int y = 1; y <= 20; y++)
			{
				int check_line = 0;
				for (int x = 1; x <= 10; x++)
				{
					if (map[y][x] == 2)
					{
						check_line++;
					}
				}
				if (check_line == SIZE_WIDTH)
				{
					arr[a++] = y;
				}
			}

			//줄 내리기
			for (int i = 0; i < 4; i++)
			{
				if (arr[i] != -1)
				{
					for (int by = arr[i]; by > 0; by--)
					{
						for (int bx = 1; bx < SIZE_WIDTH; bx++)
						{
							map[by][bx] = map[by - 1][bx];
						}
					}
				}
			}
		}
		break;
	}



	InvalidateRect(mHwnd, NULL, true);

}



int Check(int rotation, int sx, int sy)
{
	/*for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			if (block[bs][rot][x][y] == 1)
			{
				if ((sx + x == 0) || (sx + x == SIZE_WIDTH))
				{
					return -1;
				}
			}
		}
	}
	return 0;*/

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			if (block[block_shape][rotation][y][x] == 1)
			{
				if (map[sy + y][sx + x] == 1)
				{
					return -1;
				}
				else if (map[sy + y][sx + x] == 2)
				{
					return -2;
				}
			}
		}
	}
	return 0;
}
