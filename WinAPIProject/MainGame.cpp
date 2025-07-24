#include "Stdafx.h"
#include "MainGame.h"

HRESULT MainGame::init(void)
{
	GameNode::init();

	// 배경 이미지 주소값
	_bgImage = new GImage;
	_bgImage->init("Resources/Images/BackGround/DarkSky.bmp", WINSIZE_X, WINSIZE_Y);

	IMAGEMANAGER->addFrameImage("기본달리기", "Resources/Images/Object/PanCakeRun.bmp", 181 * 4, 144, 4, 1, true, RGB(255, 0, 255));

	_panCakeX = 100;
	_panCakeY = WINSIZE_Y - 194;
	_panCakeFrameX = 0;
	_panCakeFrameCount = 0;

	RND->init();

	_bgX = 0.0f;

	return S_OK;
}

void MainGame::release(void)
{
	GameNode::release();
	IMAGEMANAGER->release();

	SAFE_DELETE(_bgImage);
	//SAFE_DELETE(_plImage);
}

void MainGame::update(void)
{
	GameNode::update();

	if (KEYMANAGER->isOnceKeyDown('Q'))
	{
		if (MessageBox(_hWnd, "게임을 종료하시겠습니까?", "게임 종료", MB_OKCANCEL))
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

	if (_bgX <= -WINSIZE_X)
	{
		_bgX = 0;
	}

	_panCakeFrameCount++;
	if (_panCakeFrameCount % 10 == 0) // 애니메이션 속도 조절
	{
		_panCakeFrameX++;
		if (_panCakeFrameX >= 4) // 총 4개의 프레임
		{
			_panCakeFrameX = 0;
		}
	}
}

void MainGame::render(HDC hdc)
{
	HDC memDC = this->getBackBuffer()->getMemDC();
	PatBlt(memDC, 0, 0, WINSIZE_X, WINSIZE_Y, WHITENESS);
	// ============

	_bgImage->render(memDC, (int)_bgX, 0);
	_bgImage->render(memDC, (int)_bgX + WINSIZE_X, 0);

	IMAGEMANAGER->frameRender("기본달리기", memDC, _panCakeX, _panCakeY, _panCakeFrameX, 0);

	if (KEYMANAGER->isToggleKey(VK_F1))
	{
		//DrawRectMake(memDC, );
	}

	//_plImage->render(memDC, _playerRect.left, _playerRect.top);

	// ============
	this->getBackBuffer()->render(hdc, 0, 0);
}
