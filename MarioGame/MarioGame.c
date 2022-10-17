#define _CRT_SECURE_NO_WARNINGS
#define _WIN32_WINNT 0x0A00
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <math.h>
#pragma comment (lib, "winmm.lib")
//#include <conio.h>
//#include <string.h>
#define IDM_FILE_NEW 1
#define IDM_FILE_OPEN 2
#define IDM_FILE_QUIT 3
#define mapWidth 1300
#define mapHeight 730

static HDC tube;
static HDC stone;
static HDC mario;
static HBITMAP hBitmap;
static BITMAP bm;
static RECT rc;
HDC hdc;
PAINTSTRUCT ps;
HBITMAP hbtm;

struct SObject
{
	int xPos, yPos;
} mario1;


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)//Main window
{
	HWND hwnd;
	MSG msg;
	WNDCLASSEX wc;	

	mario1.xPos = 100;
	mario1.yPos = 535;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = CreatePatternBrush((HBITMAP)LoadImageW(NULL, (LPCWSTR)L"D:\\Nikita\\image\\background.bmp",
		IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE));
	wc.hInstance = hInstance;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.lpfnWndProc = WndProc;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"WinMainClass";



	RegisterClassEx(&wc);
	hwnd = CreateWindow(
		wc.lpszClassName,
		L"shMario",
		WS_OVERLAPPEDWINDOW,
		0, 0, mapWidth, mapHeight,
		NULL, NULL, hInstance, NULL);

	HDC hdc = GetDC(hwnd);
	ShowWindow(hwnd, nCmdShow);


	
	while (1) 
	{
		if (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) { // check the messages queue
			if (msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else 
		{
			move(hwnd);
			Sleep(50);
		}
	}
	return 0;

	return (int)msg.wParam;
}

void AddMenus(HWND hwnd) { //Menu

	HMENU hMenubar;
	HMENU hMenu;

	hMenubar = CreateMenu();
	hMenu = CreateMenu();

	AppendMenuW(hMenu, MF_STRING, IDM_FILE_NEW, L"&New");
	AppendMenuW(hMenu, MF_STRING, IDM_FILE_OPEN, L"&Open");
	AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
	AppendMenuW(hMenu, MF_STRING, IDM_FILE_QUIT, L"&Quit");

	AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenu, L"&Menu");
	SetMenu(hwnd, hMenubar);
}


LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	
		static HBITMAP hBitmap;
		HDC hdc;
		PAINTSTRUCT ps;
		BITMAP bitmap;
		HDC hdcMem;
		HGDIOBJ oldBitmap;

		wchar_t buf[40];

	
	switch (msg) {

	case WM_CREATE:



		AddMenus(hwnd);
		PlaySoundW(TEXT("D:\\Nikita\\sound\\soundmario.wav"), NULL, SND_FILENAME | SND_ASYNC);
		
		hdc = GetDC(hwnd);

	
		hBitmap = (HBITMAP)LoadImage(NULL, TEXT("D:\\Nikita\\image\\tube.bmp"), //Tube image
						   IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(hBitmap, sizeof(bm), &bm);
		tube = CreateCompatibleDC(hdc);
		SelectObject(tube, hBitmap);

		hBitmap = (HBITMAP)LoadImage(NULL, TEXT("D:\\Nikita\\image\\stone.bmp"), //Stone image
						   IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(hBitmap, sizeof(bm), &bm);
		stone = CreateCompatibleDC(hdc);
		SelectObject(stone, hBitmap);

		hBitmap = (HBITMAP)LoadImage(NULL, TEXT("D:\\Nikita\\image\\mario.bmp"), //mario image
						   IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(hBitmap, sizeof(bm), &bm);
		mario = CreateCompatibleDC(hdc);
		SelectObject(mario, hBitmap);

		//If cant load image 

		if (hBitmap == NULL) {  
			MessageBox(hwnd, L"Failed to load image", L"Error", MB_OK);
		}
		
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		hdcMem = CreateCompatibleDC(hdc);
		oldBitmap = SelectObject(hdcMem, hBitmap);

		BitBlt(hdc, 1100, 340, 157, 243, tube, 0, 0, SRCCOPY); // tube
		BitBlt(hdc, 370, 370, 66, 46, stone, 0, 0, SRCCOPY); //stone
		BitBlt(hdc, mario1.xPos, mario1.yPos, 37, 48, mario, 0, 0, SRCCOPY); //mario


		wsprintf(buf, TEXT("Mario X position %i"), mario1.xPos);


		TextOutW(hdc, 100, 100, buf, lstrlen(buf));
		InvalidateRect(hwnd, 0, 0);
		
		SelectObject(hdcMem, oldBitmap);
		DeleteDC(hdcMem);
		EndPaint(hwnd, &ps);
		
		break;

	case WM_COMMAND:

		switch (LOWORD(wparam)) {

		case IDM_FILE_NEW:
		case IDM_FILE_OPEN:

			MessageBeep(MB_ICONINFORMATION);
			break;

		case IDM_FILE_QUIT:

			SendMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		}

		break;
		
	case WM_KEYDOWN:

		if (wparam == VK_RIGHT)
		{
			mario1.xPos += 100;
			SendMessage(hwnd, WM_PAINT, 0, 0);
		}

		if (wparam == VK_ESCAPE) {

			int ret = MessageBoxW(hwnd, L"Quit?",
				L"Message", MB_OKCANCEL);

			if (ret == IDOK) {

				SendMessage(hwnd, WM_CLOSE, 0, 0);
			}
		}
		break;

	case WM_DESTROY:
		DeleteObject(hbtm);
		PostQuitMessage(0);
		break;

	}
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}


int move(HWND hwnd)
{
	if (GetKeyState('D') < 0)  mario1.xPos = mario1.xPos + 115; 
}

