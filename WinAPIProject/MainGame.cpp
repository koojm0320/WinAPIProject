#include "MainGame.h"
#include "Stdafx.h"

HRESULT MainGame::init(void)
{
	GameNode::init();

	// 배경 이미지 주소값
	_bgImage = new GImage;
	_bgImage->init("Resources/Images/BackGround/BackGround.bmp", WINSIZE_X, WINSIZE_Y);

	// 플레이어 이미지 주소값
	_plImage = new GImage;
	_plImage->init("Resources/Images/Object/Player.bmp", 60, 60, true, RGB(255, 0, 255));


	rc = RectMakeCenter(WINSIZE_X / 2, WINSIZE_Y / 2, 100, 100);

	return S_OK;
}

void MainGame::release(void)
{
	GameNode::release();

	SAFE_DELETE(_bgImage);
	SAFE_DELETE(_plImage);
}

void MainGame::update(void)
{
	GameNode::update();

	if (KEYMANAGER->isOnceKeyDown('Q'))
	{
		if (MessageBox(_hWnd, "게임 종료?", "종료확인", MB_OKCANCEL))
		{
			PostQuitMessage(0);
		}
	}

	if (KEYMANAGER->isStayKeyDown(VK_UP))
	{
		rc.top -= 3;
		rc.bottom -= 3;
	}
	if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	{
		rc.top += 3;
		rc.bottom += 3;
	}
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		rc.left -= 3;
		rc.right -= 3;
	}
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		rc.left += 3;
		rc.right += 3;
	}
}

void MainGame::render(HDC hdc)
{
	HDC memDC = this->getBackBuffer()->getMemDC();
	PatBlt(memDC, 0, 0, WINSIZE_X, WINSIZE_Y, BLACKNESS);
	// ============

	_bgImage->render(memDC, 0, 0);

	if (KEYMANAGER->isToggleKey(VK_F1))
	{
		DrawRectMake(hdc, rc);
	}

	_plImage->render(memDC, rc.left, rc.top);

	// ============
	this->getBackBuffer()->render(hdc, 0, 0);
}
