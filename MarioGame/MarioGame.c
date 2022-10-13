//#define _CRT_SECURE_NO_WARNINGS
#define _WIN32_WINNT 0x0A00
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#pragma comment (lib, "winmm.lib")
//#include <conio.h>
//#include <string.h>
#define IDM_FILE_NEW 1
#define IDM_FILE_OPEN 2
#define IDM_FILE_QUIT 3

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HBITMAP hbtm;
void LoadImageBtm(HWND hwnd, wchar_t path[]);
wchar_t background[] = L"D:\\Nikita\\image\\background.bmp";


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

	switch (msg) {

	case WM_CREATE:
		AddMenus(hwnd);
		PlaySoundW(TEXT("D:\\Nikita\\sound\\soundmario.wav"), NULL, SND_FILENAME | SND_ASYNC);
		break;

	case WM_PAINT:
		LoadImageBtm(hwnd,background);
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

		if (wparam == VK_ESCAPE) {

			int ret = MessageBoxW(hwnd, L"Are you gay?",
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





int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)//Main window
{
	HWND hwnd;
	MSG msg;
	WNDCLASSEX wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
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
		0, 0, 1215, 734,
		NULL, NULL, hInstance, NULL);

	HDC hdc = GetDC(hwnd);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	LoadImageBtm(hwnd, background);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		
	}
	return (int) msg.wParam;
}


void LoadImageBtm(HWND hwnd, wchar_t path[]) { //Load background

	HDC hdc;
	PAINTSTRUCT ps;
	BITMAP bitmap;
	HDC hdcMem;
	HGDIOBJ oldBitmap;

	hbtm = LoadImageW(NULL, path,
		IMAGE_BITMAP, 1200, 675, LR_LOADFROMFILE);
	hdc = BeginPaint(hwnd, &ps);
	hdcMem = CreateCompatibleDC(hdc);
	oldBitmap = SelectObject(hdcMem, hbtm);
	GetObject(hbtm, sizeof(bitmap), &bitmap);
	BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight,
		hdcMem, 0, 0, SRCCOPY);
	SelectObject(hdcMem, oldBitmap);
	DeleteDC(hdcMem);
	EndPaint(hwnd, &ps);
}