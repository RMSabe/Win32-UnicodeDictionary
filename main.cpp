#ifndef UNICODE
#define UNICODE
#endif

#include <cstdio>
#include <cstring>
#include <string>
#include <windows.h>

#define WNDCLASS_NAME (L"MY_WNDCLASS")
#define WINDOW_NAME (L"WIN32 Unicode Dictionary")
#define WINDOW_X_POS 50
#define WINDOW_Y_POS 50
#define WINDOW_WIDTH 720
#define WINDOW_HEIGHT 360

#define INPUT_BUFFER_SIZE_BYTES 256
#define INPUT_BUFFER_SIZE_WCHAR (INPUT_BUFFER_SIZE_BYTES/2)

#define BUFFER_SIZE_BYTES 16
#define BUFFER_SIZE_WCHAR (BUFFER_SIZE_BYTES/2)

#define RUNTIME_STATUS_IDLE (-1)
#define RUNTIME_STATUS_INIT 0
#define RUNTIME_STATUS_INPUT 1
#define RUNTIME_STATUS_CHOOSEFORMAT 2
#define RUNTIME_STATUS_OUTPUT 3

#define BUTTON_1 1
#define BUTTON_2 2

HINSTANCE hRuntimeInstance;
int runtimeStatus = -1;
int prevStatus = -1;

HWND hWindow;
HWND hText1;
HWND hText2;
HWND hTextBox;
HWND hButton1;
HWND hButton2;

std::wstring wstr = L"";
wchar_t *input_buf = NULL;
wchar_t *buf = NULL;

void paintInputScreen(void)
{
	SendMessage(hText1, WM_SETTEXT, NULL, (LPARAM) L"Enter Input Text");
	SendMessage(hTextBox, WM_SETTEXT, NULL, (LPARAM) L"");

	SetWindowPos(hButton1, NULL, 310, 200, 100, 20, 0);
	SendMessage(hButton1, WM_SETTEXT, NULL, (LPARAM) L"Proceed");

	ShowWindow(hText1, SW_SHOW);
	ShowWindow(hTextBox, SW_SHOW);
	ShowWindow(hButton1, SW_SHOW);

	ShowWindow(hText2, SW_HIDE);
	ShowWindow(hButton2, SW_HIDE);
	return;
}

void paintChooseFormatScreen(void)
{
	SendMessage(hText1, WM_SETTEXT, NULL, (LPARAM) L"Choose Output Format");

	SetWindowPos(hButton1, NULL, 250, 200, 100, 20, 0);
	SetWindowPos(hButton2, NULL, 370, 200, 100, 20, 0);

	SendMessage(hButton1, WM_SETTEXT, NULL, (LPARAM) L"Hexadecimal");
	SendMessage(hButton2, WM_SETTEXT, NULL, (LPARAM) L"Decimal");

	ShowWindow(hText1, SW_SHOW);
	ShowWindow(hButton1, SW_SHOW);
	ShowWindow(hButton2, SW_SHOW);

	ShowWindow(hText2, SW_HIDE);
	ShowWindow(hTextBox, SW_HIDE);
	return;
}

void paintOutputScreen(void)
{
	SendMessage(hText1, WM_SETTEXT, NULL, (LPARAM) L"Result");

	SetWindowPos(hButton1, NULL, 310, 200, 100, 20, 0);
	SendMessage(hButton1, WM_SETTEXT, NULL, (LPARAM) L"Return");

	ShowWindow(hText1, SW_SHOW);
	ShowWindow(hText2, SW_SHOW);
	ShowWindow(hButton1, SW_SHOW);

	ShowWindow(hTextBox, SW_HIDE);
	ShowWindow(hButton2, SW_HIDE);
	return;
}

void getTextBoxStr(void)
{
	SendMessage(hTextBox, WM_GETTEXT, (WPARAM) INPUT_BUFFER_SIZE_BYTES, (LPARAM) input_buf);
	Sleep(10);
	return;
}

void convertTextToHex(void)
{
	wstr = L"";

	unsigned int len = 0;
	while(input_buf[len] != '\0') len++;

	unsigned int nwchar = 0;

	while(nwchar < len)
	{
		swprintf(buf, BUFFER_SIZE_BYTES, L"0x%X", input_buf[nwchar]);
		wstr += buf;
		wstr += L" , ";
		nwchar++;
	}

	wstr += L"NULL";
	SendMessage(hText2, WM_SETTEXT, NULL, (LPARAM) wstr.c_str());
	return;
}

void convertTextToDec(void)
{
	wstr = L"";

	unsigned int len = 0;
	while(input_buf[len] != '\0') len++;

	unsigned int nwchar = 0;

	while(nwchar < len)
	{
		wstr += std::to_wstring((unsigned short) input_buf[nwchar]);
		wstr += L" , ";
		nwchar++;
	}

	wstr += L"NULL";
	SendMessage(hText2, WM_SETTEXT, NULL, (LPARAM) wstr.c_str());
	return;
}

void runtimeProcedure(void)
{
	switch(runtimeStatus)
	{
		case RUNTIME_STATUS_IDLE:
			Sleep(10);
			break;

		case RUNTIME_STATUS_INIT:
			ShowWindow(hWindow, SW_SHOW);
			runtimeStatus = RUNTIME_STATUS_INPUT;
			break;

		case RUNTIME_STATUS_INPUT:
			paintInputScreen();
			prevStatus = RUNTIME_STATUS_INPUT;
			runtimeStatus = RUNTIME_STATUS_IDLE;
			break;

		case RUNTIME_STATUS_CHOOSEFORMAT:
			paintChooseFormatScreen();
			prevStatus = RUNTIME_STATUS_CHOOSEFORMAT;
			runtimeStatus = RUNTIME_STATUS_IDLE;
			break;

		case RUNTIME_STATUS_OUTPUT:
			paintOutputScreen();
			prevStatus = RUNTIME_STATUS_OUTPUT;
			runtimeStatus = RUNTIME_STATUS_IDLE;
			break;
	}

	return;
}

void createElements(void)
{
	DWORD windowStyle = 0;
	LOGFONT logFont;
	HFONT hFont;
	ZeroMemory(&logFont, sizeof(LOGFONT));

	windowStyle = (WS_CHILD | SS_CENTER);
	hText1 = CreateWindow(L"STATIC", NULL, windowStyle, 60, 50, 600, 35, hWindow, NULL, hRuntimeInstance, NULL);
	logFont.lfCharSet = DEFAULT_CHARSET;
	logFont.lfWidth = 20;
	logFont.lfHeight = 35;
	logFont.lfWeight = FW_NORMAL;
	hFont = CreateFontIndirect(&logFont);
	SendMessage(hText1, WM_SETFONT, (WPARAM) hFont, (LPARAM) TRUE);

	windowStyle = WS_CHILD;
	hText2 = CreateWindow(L"STATIC", NULL, windowStyle, 60, 100, 600, 48, hWindow, NULL, hRuntimeInstance, NULL);
	logFont.lfWidth = 8;
	logFont.lfHeight = 16;
	hFont = CreateFontIndirect(&logFont);
	SendMessage(hText2, WM_SETFONT, (WPARAM) hFont, (LPARAM) TRUE);

	hTextBox = CreateWindow(L"EDIT", NULL, windowStyle, 60, 100, 600, 60, hWindow, NULL, hRuntimeInstance, NULL);

	hButton1 = CreateWindow(L"BUTTON", NULL, windowStyle, 0, 0, 0, 0, hWindow, (HMENU) BUTTON_1, hRuntimeInstance, NULL);
	hButton2 = CreateWindow(L"BUTTON", NULL, windowStyle, 0, 0, 0, 0, hWindow, (HMENU) BUTTON_2, hRuntimeInstance, NULL);

	return;
}

void destroyElements(void)
{
	DestroyWindow(hText1);
	DestroyWindow(hText2);
	DestroyWindow(hTextBox);
	DestroyWindow(hButton1);
	DestroyWindow(hButton2);

	return;
}

void createMainWindow(void)
{
	DWORD windowStyle = (WS_CAPTION | WS_SYSMENU | WS_OVERLAPPED);
	RECT rectangle = {
		.left = WINDOW_X_POS,
		.top = WINDOW_Y_POS,
		.right = WINDOW_X_POS + WINDOW_WIDTH,
		.bottom = WINDOW_Y_POS + WINDOW_HEIGHT
	};
	AdjustWindowRect(&rectangle, windowStyle, FALSE);

	int xPos = rectangle.left;
	int yPos = rectangle.top;
	int width = rectangle.right - rectangle.left;
	int height = rectangle.bottom - rectangle.top;

	hWindow = CreateWindow(WNDCLASS_NAME, WINDOW_NAME, windowStyle, xPos, yPos, width, height, NULL, NULL, hRuntimeInstance, NULL);
	return;
}

LRESULT CALLBACK windowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	WORD arg = LOWORD(wParam);
	switch(msg)
	{
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;

		case WM_COMMAND:
			switch(prevStatus)
			{
				case RUNTIME_STATUS_INPUT:
					if(arg == BUTTON_1)
					{
						getTextBoxStr();
						runtimeStatus = RUNTIME_STATUS_CHOOSEFORMAT;
						return 0;
					}
					break;

				case RUNTIME_STATUS_CHOOSEFORMAT:
					if(arg == BUTTON_1)
					{
						convertTextToHex();
						runtimeStatus = RUNTIME_STATUS_OUTPUT;
						return 0;
					}
					else if(arg == BUTTON_2)
					{
						convertTextToDec();
						runtimeStatus = RUNTIME_STATUS_OUTPUT;
						return 0;
					}
					break;

				case RUNTIME_STATUS_OUTPUT:
					if(arg == BUTTON_1)
					{
						runtimeStatus = RUNTIME_STATUS_INPUT;
						return 0;
					}
					break;
			}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void registerWndClass(void)
{
	WNDCLASS myWndClass;
	ZeroMemory(&myWndClass, sizeof(WNDCLASS));
	myWndClass.hInstance = hRuntimeInstance;
	myWndClass.lpszClassName = WNDCLASS_NAME;
	myWndClass.lpfnWndProc = windowProcedure;
	myWndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	myWndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	myWndClass.hbrBackground = (HBRUSH) GetStockObject(LTGRAY_BRUSH);
	RegisterClass(&myWndClass);
	return;
}

void initialize(void)
{
	input_buf = (wchar_t*) malloc(INPUT_BUFFER_SIZE_BYTES);
	buf = (wchar_t*) malloc(BUFFER_SIZE_BYTES);

	memset(input_buf, 0, INPUT_BUFFER_SIZE_BYTES);
	memset(buf, 0, BUFFER_SIZE_BYTES);

	registerWndClass();
	createMainWindow();
	createElements();

	runtimeStatus = RUNTIME_STATUS_INIT;
	return;
}

void terminate(void)
{
	destroyElements();
	DestroyWindow(hWindow);
	UnregisterClass(WNDCLASS_NAME, hRuntimeInstance);

	free(input_buf);
	free(buf);

	return;
}

WINBOOL runtimeContinue(void)
{
	MSG msg;
	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if(msg.message == WM_QUIT) return FALSE;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return TRUE;
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
	hRuntimeInstance = hInstance;
	initialize();
	while(runtimeContinue()) runtimeProcedure();
	terminate();
	return 0;
}
