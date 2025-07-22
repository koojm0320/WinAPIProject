#include "Stdafx.h"
#include "MainGame.h"

HINSTANCE _hInstance;
HWND _hWnd;
POINT _ptMouse;

void setWindowSize(int x, int y, int width, int height);

MainGame* _mg;
int centerX, centerY;
RECT _rc1, _rc2;

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LPTSTR _lpszClass = TEXT("Window API");

int APIENTRY wWinMain(
	HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	LPWSTR		lpCmdLine,
	int			nCmdShow
)
{
	_mg = new MainGame();
	_hInstance = hInstance;

	WNDCLASS wndClass;

	wndClass.cbClsExtra = 0;			// 클래스 여분 메모리
	wndClass.cbWndExtra = 0;			// 윈도우 여분 메모리
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// 배경 색상
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);		// 커서(C스타일 맞춰서 NULL로 써주자)
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	// 아이콘
	wndClass.hInstance = hInstance;		// 식별자 정보
	wndClass.lpfnWndProc = WndProc;		// 프로시저
	wndClass.lpszClassName = _lpszClass;		// 클래스 이름
	wndClass.lpszMenuName = NULL;				// 메뉴 이름
	wndClass.style = CS_HREDRAW | CS_VREDRAW;	// 스타일 (다시 그리기 정보)

	RegisterClass(&wndClass);

	// 1-3. 화면에 보여줄 창 생성
	_hWnd = CreateWindow(
		WINNAME,
		WINNAME,
		WS_OVERLAPPEDWINDOW,
		WINSTART_X,
		WINSTART_Y,
		WINSIZE_X,
		WINSIZE_Y,
		NULL,
		(HMENU)NULL,
		hInstance,
		NULL);

	setWindowSize(WINSTART_X, WINSTART_Y, WINSIZE_X, WINSIZE_Y);

	// 1-4. 창 호출
	ShowWindow(_hWnd, nCmdShow);
	//UpdateWindow(_hWnd);  프레임워크 구축 없으면 사용한다.

	if (FAILED(_mg->init()))
		return 0;

	MSG message;


	while (GetMessage(&message, 0, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	_mg->release();
	UnregisterClass(WINNAME, hInstance);
	return (int)message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return _mg->mainProc(hWnd, iMessage, wParam, lParam);
}
void setWindowSize(int x, int y, int width, int height)
{
	RECT rc = { 0,0,width, height };
	AdjustWindowRect(&rc, WINSTYLE, false);
	SetWindowPos(_hWnd, NULL, x, y,
		(rc.right - rc.left),
		(rc.bottom - rc.top),
		SWP_NOZORDER | SWP_NOMOVE);
}