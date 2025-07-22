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

	wndClass.cbClsExtra = 0;			// Ŭ���� ���� �޸�
	wndClass.cbWndExtra = 0;			// ������ ���� �޸�
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// ��� ����
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);		// Ŀ��(C��Ÿ�� ���缭 NULL�� ������)
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	// ������
	wndClass.hInstance = hInstance;		// �ĺ��� ����
	wndClass.lpfnWndProc = WndProc;		// ���ν���
	wndClass.lpszClassName = _lpszClass;		// Ŭ���� �̸�
	wndClass.lpszMenuName = NULL;				// �޴� �̸�
	wndClass.style = CS_HREDRAW | CS_VREDRAW;	// ��Ÿ�� (�ٽ� �׸��� ����)

	RegisterClass(&wndClass);

	// 1-3. ȭ�鿡 ������ â ����
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

	// 1-4. â ȣ��
	ShowWindow(_hWnd, nCmdShow);
	//UpdateWindow(_hWnd);  �����ӿ�ũ ���� ������ ����Ѵ�.

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