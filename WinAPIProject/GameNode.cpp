#include "Stdafx.h"
#include "GameNode.h"

void GameNode::setBackBuffer(void)
{
	_backBuffer = new GImage;
	_backBuffer->init(WINSIZE_X, WINSIZE_Y);
}

HRESULT GameNode::init(void)
{
	SetTimer(_hWnd, 1, 10, NULL);
	KEYMANAGER->init();
	RND->init();
	this->setBackBuffer();
	// 석세스 오케이
	return S_OK;
}

void GameNode::release(void)
{
	KillTimer(_hWnd, 1);
	RND->releaseSingleton();
	KEYMANAGER->releaseSingleton();
	SAFE_DELETE(_backBuffer);
}

void GameNode::update(void)
{

	InvalidateRect(_hWnd, NULL, false);
}

void GameNode::render(HDC hdc)
{
	TCHAR strPT[1024];
	wsprintf(strPT, "%d", GetTickCount() / 1000);
	TextOut(hdc, 10, 10, strPT, strlen(strPT));
}

LRESULT GameNode::mainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMessage)
	{
	case WM_TIMER:
		this->update();
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		this->render(hdc);

		EndPaint(hWnd, &ps);
		break;
	case WM_MOUSEMOVE:
		_ptMouse.x = LOWORD(lParam);
		_ptMouse.y = HIWORD(lParam);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostMessage(hWnd, WM_DESTROY, 0, 0);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

GameNode::GameNode()
{
}

GameNode::~GameNode()
{
}

