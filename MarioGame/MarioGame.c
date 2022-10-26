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
#define mapWidth 1216
#define mapHeight 730
#define IdBaseTimer 1

static HDC background;		//1216 x 730 
static HDC tube;			//157 x 243
static HDC stone;			//66 x 46
static HDC mario;			//37 x 48
static HDC marioL;			//37 x 48
static HDC walk;			//37 x 48
static HDC walkL;			//37 x 48
static HDC jumping;			//37 x 48
static HDC jumpingL;		//37 x 48

static HBITMAP hBitmap;
static BITMAP bm;
static RECT rc;

HWND hwnd;
HDC hdc;
PAINTSTRUCT ps;
HBITMAP hbtm;


typedef struct SObject            // Main struct of mario
{
	int xPos, yPos;
	int runAnimation;
	int speedX, speedY;
	BOOL JumpController;

} Mario, *Pmario;

int JumpBase;

void Jump(Pmario pmario)		 // Gravity on earth
{
	if (!pmario->JumpController) {
		JumpBase = pmario->yPos;
		pmario->JumpController = TRUE;
		pmario->speedY = -50;
	}
}

void MarioMove(Pmario pmario) {
	Move(hwnd);
	if (pmario->runAnimation == 3) pmario->runAnimation = 0;
	if (pmario->yPos - JumpBase <= -200) pmario->speedY *= -1;
	pmario->xPos += pmario->speedX;
	if (pmario->speedX != 0) pmario->runAnimation = pmario->runAnimation++;
	pmario->yPos += pmario->speedY;
	if (pmario->yPos == JumpBase) {
		pmario->JumpController = FALSE;
		pmario->speedY = 0;
	}
}

Mario mario1;

typedef struct TObject  //main struct of walls 
{
	int x, y;
	int speedX, speedY;

} wall, *Pwall;


Pwall wallArray = NULL;
int PwallArrayCounter = 0;


void newWall(int x, int y)
{
	wall wallBrick = { x,y,0,0 };
	PwallArrayCounter++;
	wallArray = realloc(wallArray, (sizeof(*wallArray) * PwallArrayCounter)); 
	wallArray[PwallArrayCounter - 1] = wallBrick;

}


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)	//Main window
{
	MSG msg;
	WNDCLASSEX wc;


	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hIcon = LoadIcon(hInstance, IDI_WINLOGO);
	wc.hIconSm = LoadIcon(hInstance, IDI_WINLOGO);;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		//CreatePatternBrush((HBITMAP)LoadImageW(NULL, (LPCWSTR)L"D:\\Nikita\\image\\background.bmp",
		//IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE));
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

		
	while (1)												// Main loop
	{
		if (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {	// Check the messages queue
			if (msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else 
		{
			drow(hdc);
			InvalidateRect(hwnd, 0, 0);
			Sleep(75);
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

	
	switch (msg) {

	case WM_CREATE:

		SetTimer(hwnd, IdBaseTimer, 10, NULL);

		AddMenus(hwnd);
		PlaySoundW(TEXT(".\\sound\\soundmario.wav"), NULL, SND_FILENAME | SND_ASYNC);
		
		hdc = GetDC(hwnd);

		mario1.xPos = 100;
		mario1.yPos = 535;
		mario1.JumpController = FALSE;
		mario1.runAnimation = 0;


		newWall(370, 410);
		newWall(436, 410);
		newWall(502, 410);
		newWall(568, 410);
		newWall(634, 410);

		newWall(482, 230);
		newWall(548, 230);
		newWall(614, 230);



		hBitmap = (HBITMAP)LoadImage(NULL, TEXT(".\\image\\background.bmp"),			//Background image
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(hBitmap, sizeof(bm), &bm);
		background = CreateCompatibleDC(hdc);
		SelectObject(background, hBitmap);
	
		hBitmap = (HBITMAP)LoadImage(NULL, TEXT(".\\image\\tube.bmp"),				    //Tube image
						   IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(hBitmap, sizeof(bm), &bm);
		tube = CreateCompatibleDC(hdc);
		SelectObject(tube, hBitmap);

		hBitmap = (HBITMAP)LoadImage(NULL, TEXT(".\\image\\stone.bmp"),					//Stone image
						   IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(hBitmap, sizeof(bm), &bm);
		stone = CreateCompatibleDC(hdc);
		SelectObject(stone, hBitmap);

		hBitmap = (HBITMAP)LoadImage(NULL, TEXT(".\\image\\mario.bmp"),					//mario image
						   IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(hBitmap, sizeof(bm), &bm);
		mario = CreateCompatibleDC(hdc);
		SelectObject(mario, hBitmap);

		hBitmap = (HBITMAP)LoadImage(NULL, TEXT(".\\image\\marioL.bmp"),				//mario left image
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(hBitmap, sizeof(bm), &bm);
		marioL = CreateCompatibleDC(hdc);
		SelectObject(marioL, hBitmap);

		hBitmap = (HBITMAP)LoadImage(NULL, TEXT(".\\image\\walk.bmp"),					//run image
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(hBitmap, sizeof(bm), &bm);
		walk = CreateCompatibleDC(hdc);
		SelectObject(walk, hBitmap);

		hBitmap = (HBITMAP)LoadImage(NULL, TEXT(".\\image\\walkL.bmp"),					//run to left image
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(hBitmap, sizeof(bm), &bm);
		walkL = CreateCompatibleDC(hdc);
		SelectObject(walkL, hBitmap);

		hBitmap = (HBITMAP)LoadImage(NULL, TEXT(".\\image\\jumping.bmp"),				//jumping image
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(hBitmap, sizeof(bm), &bm);
		jumping = CreateCompatibleDC(hdc);
		SelectObject(jumping, hBitmap);

		hBitmap = (HBITMAP)LoadImage(NULL, TEXT(".\\image\\jumpingL.bmp"),				//jumping to left image
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(hBitmap, sizeof(bm), &bm);
		jumpingL = CreateCompatibleDC(hdc);
		SelectObject(jumpingL, hBitmap);
		

		if (hBitmap == NULL) {															//If cant load image 
			MessageBox(hwnd, L"Failed to load image", L"Error", MB_OK);
		}
		
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		
		drow(hdc);

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

		/*if (wparam == VK_RIGHT)
		{
			mario1.xPos += 100;
			SendMessage(hwnd, WM_PAINT, 0, 0);
		}*/

		if (wparam == VK_ESCAPE) {

			int ret = MessageBoxW(hwnd, L"Quit?",
				L"Message", MB_OKCANCEL);

			if (ret == IDOK) {

				SendMessage(hwnd, WM_CLOSE, 0, 0);
			}
		}
		break;

	case WM_TIMER:
		switch (wparam)
		{
		case IdBaseTimer:
			MarioMove(&mario1);
			break;
		} break;


	case WM_DESTROY:
		DeleteObject(hbtm);
		PostQuitMessage(0);
		break;

	}
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}


int drow(HDC hdc)
{
	HDC hdcMem;
	wchar_t bufY[40];
	wchar_t bufX[40];

	HGDIOBJ oldBitmap;

	hdcMem = CreateCompatibleDC(hdc);
	oldBitmap = SelectObject(hdcMem, hBitmap);

	BitBlt(hdc, 0, 0, mapWidth, mapHeight, background, 0, 0, SRCCOPY);	// background
	BitBlt(hdc, 1035, 340, 157, 243, tube, 0, 0, SRCCOPY);				// tube
	

	for (int i= 0; i < PwallArrayCounter; i++) {
		BitBlt(hdc, wallArray[i].x, wallArray[i].y, 66, 46, stone, 0, 0, SRCCOPY);				// stone
	}
	
	
	if (!mario1.speedX) mario1.runAnimation = 0;									//animation mario when he runnig
	
	if (mario1.runAnimation == 0 || mario1.runAnimation == 1){
		if (mario1.speedX < 0) 
			 BitBlt(hdc, mario1.xPos, mario1.yPos, 37, 48, marioL, 0, 0, SRCCOPY);
		else BitBlt(hdc, mario1.xPos, mario1.yPos, 37, 48, mario, 0, 0, SRCCOPY);      
	}
	else if (mario1.runAnimation == 2 || mario1.runAnimation == 3){
		if (mario1.speedX <0 ) 
			 BitBlt(hdc, mario1.xPos, mario1.yPos, 37, 48, walkL, 0, 0, SRCCOPY);    
		else BitBlt(hdc, mario1.xPos, mario1.yPos, 37, 48, walk, 0, 0, SRCCOPY);

	}
	
	
	if (mario1.JumpController == TRUE && mario1.speedX >= 0) {						//animation mario when he jumping
		BitBlt(hdc, mario1.xPos, mario1.yPos, 37, 48, jumping, 0, 0, SRCCOPY);
	}
	if (mario1.JumpController == TRUE && mario1.speedX < 0) {
		BitBlt(hdc, mario1.xPos, mario1.yPos, 37, 48, jumpingL, 0, 0, SRCCOPY);
	}

	wsprintf(bufX, TEXT("Mario X position %i"), mario1.xPos);					    //Show position mario on screen
	wsprintf(bufY, TEXT("Mario Y position %i"), mario1.yPos);


	TextOutW(hdc, 100, 100, bufX, lstrlen(bufX));
	TextOutW(hdc, 100, 130, bufY, lstrlen(bufY));


	SelectObject(hdcMem, oldBitmap);
	DeleteDC(hdcMem);
}


int Move(HWND hwnd)          //X 0 Y 535
{
	mario1.speedX = 0;
	if (GetKeyState('A') < 0 && mario1.xPos > 0)  mario1.speedX = - 20;
	if (GetKeyState('D') < 0 && mario1.xPos < 1160)  mario1.speedX = 20;
	if (GetKeyState(VK_SPACE) < 0) Jump(&mario1);
}


