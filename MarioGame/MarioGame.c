//#define _CRT_SECURE_NO_WARNINGS
#define _WIN32_WINNT 0x0A00
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
//#include <conio.h>
//#include <string.h>
#define IDM_FILE_NEW 1
#define IDM_FILE_OPEN 2
#define IDM_FILE_QUIT 3

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HBITMAP hBitmap;

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
	static HDC background;
	static BITMAP bm;
	HDC hdc = GetDC(hwnd);
	HWND hsti;


	switch (msg) {

	case WM_DESTROY: 
		DeleteObject(hBitmap);
		PostQuitMessage(0);
		break;
	

	case WM_CREATE:
		AddMenus(hwnd);
		hBitmap = (HBITMAP)LoadImage(NULL, TEXT("C:\\Users\\Bubaleg\\Desktop\\papka govna\\imagebackground.bmp")
												, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(hBitmap, sizeof(bm), &bm);
		background = CreateCompatibleDC(hdc);
		SelectObject(background, hBitmap);
	/*	hsti = CreateWindowW(L"Static", L"",
			WS_CHILD | WS_VISIBLE | SS_BITMAP,
			5, 5, 300, 300, hwnd, (HMENU)1, NULL, NULL);

		SendMessage(hsti, STM_SETIMAGE,
			(WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap);*/
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
		0, 0, 1200, 675,
		NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}



