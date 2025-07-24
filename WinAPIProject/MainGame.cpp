#include "Stdafx.h"
#include "MainGame.h"

HRESULT MainGame::init(void)
{
	GameNode::init();

	// ��� �̹��� �ּҰ�
	_bgImage = new GImage;
	_bgImage->init("Resources/Images/BackGround/BackGround.bmp", WINSIZE_X, WINSIZE_Y);

	_bgObjImage1 = new GImage;
	_bgObjImage1->init("Resources/Images/BackGround/BackGroundObject1.bmp", WINSIZE_X, WINSIZE_Y, true, RGB(255, 0, 255));

	_bgObjImage2 = new GImage;
	_bgObjImage2->init("Resources/Images/BackGround/BackGroundObject2.bmp", WINSIZE_X, WINSIZE_Y, true, RGB(255, 0, 255));

	
	IMAGEMANAGER->addFrameImage("�⺻�޸���", "Resources/Images/Object/PanCakeRun.bmp", 181 * 4, 144, 4, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("�⺻�����̵�", "Resources/Images/Object/PanCakeSlide.bmp", 324, 108, 2, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("����", "Resources/Images/Object/PanCakeJump.bmp", 334, 140, 2, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("��������", "Resources/Images/Object/PanCakeDoubleJump.bmp", 1062, 146, 6, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("����", "Resources/Images/Object/PanCakeLanding.bmp", 150, 126, 1, 1, true, RGB(255, 0, 255));


	_panCakeX = 130;
	_groundY = WINSIZE_Y - 250;
	_panCakeY = _groundY;
	_bgX = 0.0f;
	_bgObj1X = 0.0f;
	_bgObj2X = 0.0f;

	// ���� ����
	_playerState = PlayerState::RUNNING;
	_jumpPower = 18.0f;
	_gravity = 1.0f;
	_velocityY = 0.0f;
	_canDoubleJump = false;
	_landingTime = 0.15f; // 0.15�ʰ� ���� ���
	_landingTimer = 0.0f;


	_panCakeFrameX = 0;
	_panCakeFrameCount = 0;



	RND->init();
	return S_OK;
}

void MainGame::release(void)
{
	GameNode::release();
	IMAGEMANAGER->release();

	SAFE_DELETE(_bgImage);
	SAFE_DELETE(_bgObjImage1);
	SAFE_DELETE(_bgObjImage2);
}

void MainGame::update(void)
{
	GameNode::update();

	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
	{
		switch (_playerState)
		{
		case PlayerState::RUNNING:
		case PlayerState::SLIDING:
		case PlayerState::LANDING:
			_playerState = PlayerState::JUMPING;
			_velocityY = -_jumpPower;
			_canDoubleJump = true;
			_panCakeFrameX = 0; // ���� �ִϸ��̼� ������ �ʱ�ȭ
			_panCakeFrameCount = 0;
			break;
		case PlayerState::JUMPING:
			if (_canDoubleJump)
			{
				_playerState = PlayerState::DOUBLE_JUMPING;
				_velocityY = -_jumpPower;
				_canDoubleJump = false;
				_panCakeFrameX = 0; // ���� ���� �ִϸ��̼� ������ �ʱ�ȭ
				_panCakeFrameCount = 0;
			}
			break;
		}
	}

	// === ���¿� ���� ���� ó�� ===
	// ���� �Ǵ� ���� ���� ������ �� ���� ȿ�� ����
	if (_playerState == PlayerState::JUMPING || _playerState == PlayerState::DOUBLE_JUMPING)
	{
		_velocityY += _gravity;
		_panCakeY += _velocityY;

		// ���� ��Ҵ��� Ȯ��
		if (_panCakeY >= _groundY)
		{
			_panCakeY = _groundY;
			_velocityY = 0;
			_playerState = PlayerState::LANDING;
			_landingTimer = _landingTime; // ���� Ÿ�̸� ����
		}
	}
	else if (_playerState == PlayerState::LANDING)
	{
		// ���� Ÿ�̸� ����
		if (KEYMANAGER->isStayKeyDown(VK_SHIFT))
		{
			_playerState = PlayerState::SLIDING;
			_panCakeFrameX = 0;
			_panCakeFrameCount = 0;

		}
		else
		{
			_landingTimer -= 1.0f / 60.0f; // 60������ ���� �ð� ����
			if (_landingTimer <= 0)
			{
				_playerState = PlayerState::RUNNING; // ���� �� �޸��� ���·� ��ȯ
			}
		}
	}
	else
	{
		// ���� ���� �� �����̵� ó��
		if (KEYMANAGER->isStayKeyDown(VK_SHIFT))
		{
			if (_playerState == PlayerState::RUNNING)
			{
				_playerState = PlayerState::SLIDING;
				_panCakeFrameX = 0;
				_panCakeFrameCount = 0;
			}
		}
		else if (_playerState == PlayerState::SLIDING)
		{
			_playerState = PlayerState::RUNNING;
			_panCakeFrameX = 0;
			_panCakeFrameCount = 0;
		}
	}

	// �޹�� �ӵ�ó��
	_bgX += -1.0f;
	if (_bgX <= -WINSIZE_X)
	{
		_bgX = 0;
	}

	_bgObj1X += -1.5f;
	if (_bgObj1X <= -WINSIZE_X)
	{
		_bgObj1X = 0;
	}

	_bgObj2X += -2.0f;
	if (_bgObj2X <= -WINSIZE_X)
	{		  
		_bgObj2X = 0;
	}

	// �� �ִϸ��̼� �� ������ �ӵ� ó��
	_panCakeFrameCount++;
	switch (_playerState)
	{
	case PlayerState::RUNNING:
		if (_panCakeFrameCount % 5 == 0) _panCakeFrameX = (_panCakeFrameX + 1) % 4;
		break;
	case PlayerState::SLIDING:
		if (_panCakeFrameCount % 5 == 0) _panCakeFrameX = (_panCakeFrameX + 1) % 2;
		break;
	case PlayerState::JUMPING:
		if (_panCakeFrameCount % 5 == 0) _panCakeFrameX = (_panCakeFrameX + 1) % 2;
		break;
	case PlayerState::DOUBLE_JUMPING:
		if (_panCakeFrameCount % 4 == 0) _panCakeFrameX = (_panCakeFrameX + 1) % 6;
		break;
	case PlayerState::LANDING:
		_panCakeFrameX = 0;
		break;
	}
}

void MainGame::render(HDC hdc)
{
	HDC memDC = this->getBackBuffer()->getMemDC();
	PatBlt(memDC, 0, 0, WINSIZE_X, WINSIZE_Y, WHITENESS);
	// ============

	_bgImage->render(memDC, (int)_bgX, 0);
	_bgImage->render(memDC, (int)_bgX + WINSIZE_X, 0);

	_bgObjImage1->render(memDC, (int)_bgObj1X, 0);
	_bgObjImage1->render(memDC, (int)_bgObj1X + WINSIZE_X, 0);

	_bgObjImage2->render(memDC, (int)_bgObj2X, 0);
	_bgObjImage2->render(memDC, (int)_bgObj2X + WINSIZE_X, 0);

	switch (_playerState)
	{
	case PlayerState::RUNNING:
		IMAGEMANAGER->frameRender("�⺻�޸���", memDC, _panCakeX, (int)_panCakeY, _panCakeFrameX, 0);
		break;
	case PlayerState::SLIDING:
		IMAGEMANAGER->frameRender("�⺻�����̵�", memDC, _panCakeX, (int)_panCakeY + (144 - 108), _panCakeFrameX, 0);
		break;
	case PlayerState::JUMPING:
		IMAGEMANAGER->frameRender("����", memDC, _panCakeX, (int)_panCakeY + (144 - 140), _panCakeFrameX, 0);
		break;
	case PlayerState::DOUBLE_JUMPING:
		IMAGEMANAGER->frameRender("��������", memDC, _panCakeX, (int)_panCakeY + (144 - 146), _panCakeFrameX, 0);
		break;
	case PlayerState::LANDING:
		IMAGEMANAGER->frameRender("����", memDC, _panCakeX, (int)_panCakeY + (144 - 126), _panCakeFrameX, 0);
		break;
	}


	// ============
	this->getBackBuffer()->render(hdc, 0, 0);
}
