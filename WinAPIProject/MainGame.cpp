#include "Stdafx.h"
#include "MainGame.h"

HRESULT MainGame::init(void)
{
	GameNode::init();

	// ��� �̹��� �ּҰ�
	_bgImage = new GImage;
	_bgImage->init("Resources/Images/BackGround/DarkSky.bmp", WINSIZE_X, WINSIZE_Y);



	// �÷��̾� �̹��� �ּҰ�
	//_plImage = new GImage;
	//_plImage->init("Resources/Images/Object/Player.bmp", 70, 70, true, RGB(255, 0, 255));

	RND->init();

	_bgX = 0.0f;

	return S_OK;
}

void MainGame::release(void)
{
	GameNode::release();

	SAFE_DELETE(_bgImage);
	//SAFE_DELETE(_plImage);
}

void MainGame::update(void)
{
	GameNode::update();

	if (KEYMANAGER->isOnceKeyDown('Q'))
	{
		if (MessageBox(_hWnd, "������ �����Ͻðڽ��ϱ�?", "���� ����", MB_OKCANCEL))
		{
			PostQuitMessage(0);
		}
	}

	if (KEYMANAGER->isStayKeyDown(VK_UP))
	{
	}
	if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	{
	}
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
	}
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
	}

	_bgX += -10.0f;

	if (_bgX >= WINSIZE_X)
	{
		_bgX = 0;
	}
}

void MainGame::render(HDC hdc)
{
	HDC memDC = this->getBackBuffer()->getMemDC();
	PatBlt(memDC, 0, 0, WINSIZE_X, WINSIZE_Y, WHITENESS);
	// ============

	_bgImage->render(memDC, (int)_bgX, 0);
	_bgImage->render(memDC, (int)_bgX + WINSIZE_X, 0);

	if (KEYMANAGER->isToggleKey(VK_F1))
	{
		//DrawRectMake(memDC, );
	}

	//_plImage->render(memDC, _playerRect.left, _playerRect.top);

	// ============
	this->getBackBuffer()->render(hdc, 0, 0);
}
