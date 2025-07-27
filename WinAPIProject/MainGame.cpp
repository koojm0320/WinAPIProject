#include "Stdafx.h"
#include "MainGame.h"

// ��� ���� �ʱ�ȭ
MainGame::MainGame() :
	_panCakeX(0), _panCakeY(0.0f),
	_panCakeFrameX(0), _panCakeFrameCount(0),
	_bgX(0.0f), _bgObj1X(0.0f), _bgObj2X(0.0f), _bgObj3X(0.0f),
	_jumpPower(0.0f), _gravity(0.0f), _velocityY(0.0f),
	_canDoubleJump(false), _landingTime(0.0f), _landingTimer(0.0f),
	_isDebug(false), _mapPosX(0.0f), _playerHitbox(RectMake(0, 0, 0, 0))

{
}

HRESULT MainGame::init(void)
{
	GameNode::init();
	IMAGEMANAGER->init();

	_hurdleManager = new HurdleManager();
	_hurdleManager->init();

	// ��� + �����
	IMAGEMANAGER->addImage("���", "Resources/Images/BackGround/BackGround.bmp", WINSIZE_X, WINSIZE_Y);
	IMAGEMANAGER->addImage("�����1", "Resources/Images/BackGround/BackGroundObject1.bmp", WINSIZE_X, WINSIZE_Y, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("�����2", "Resources/Images/BackGround/BackGroundObject2.bmp", WINSIZE_X, WINSIZE_Y, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("�����3", "Resources/Images/BackGround/BackGroundObject3.bmp", WINSIZE_X, WINSIZE_Y, true, RGB(255, 0, 255));
	

	// �ִϸ��̼�
	IMAGEMANAGER->addFrameImage("�⺻�޸���", "Resources/Images/Object/PanCakeRun.bmp", 181 * 4, 144, 4, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("�⺻�����̵�", "Resources/Images/Object/PanCakeSlide.bmp", 324, 108, 2, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("����", "Resources/Images/Object/PanCakeJump.bmp", 334, 140, 2, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("��������", "Resources/Images/Object/PanCakeDoubleJump.bmp", 1062, 146, 6, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("����", "Resources/Images/Object/PanCakeLanding.bmp", 150, 126, 1, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("�浹", "Resources/Images/Object/PanCakeCollision.bmp", 1200, 158, 6, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("���ӿ���", "Resources/Images/Object/PanCakeGameOver.bmp", 915, 146, 5, 1, true, RGB(255, 0, 255));

	// ��ֹ�
	IMAGEMANAGER->addFrameImage("����غ�", "Resources/Images/Object/hurdleReady.bmp", 138, 131, 2, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("�������", "Resources/Images/Object/hurdle.bmp", 388, 148, 4, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("�������", "Resources/Images/Object/highHurdle.bmp", 475, 296, 5, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("������ũ", "Resources/Images/Object/spike2.bmp", 137, 416, true, RGB(255, 0, 255));


	// Ÿ��
	IMAGEMANAGER->addImage("Ÿ��", "Resources/Images/Object/tile.bmp", 129, 50, true, RGB(255, 0, 255));

	// UI
	IMAGEMANAGER->addImage("ü�¾�����", "Resources/Images/UI/iconHeartLife.bmp", 54, 54, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("ü�¹ٹ��", "Resources/Images/UI/gaugebgHeart.bmp", 650, 25, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("ü�¹�", "Resources/Images/UI/gaugeHeartOrange.bmp", 644, 23, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("ü�¹�����Ʈ", "Resources/Images/UI/gaueHeartEffect.bmp", 33, 30, 2, 1, true, RGB(255, 0, 255));


	_panCakeX = 130;
	//_groundY = WINSIZE_Y - 250;
	_panCakeY = WINSIZE_Y - 250;;
	_bgX = 0.0f;
	_bgObj1X = 0.0f;
	_bgObj2X = 0.0f;
	_bgObj3X = 0.0f;

	// ���� ����
	_playerState = PlayerState::RUNNING;
	_jumpPower = 20.0f;
	_gravity = 0.97f;
	_velocityY = 0.0f;
	_canDoubleJump = false;
	_landingTime = 0.1f;
	_landingTimer = 0.0f;

	_panCakeFrameX = 0;
	_panCakeFrameCount = 0;

	_isInvincible = false;
	_invincibleTimer = 0.0f;
	_mapPosX = 0;

	_hitAnimationFinished = false;
	_gameOverAnimationFinished = false;

	_maxHp = 100.0f;
	_currentHp = _maxHp;
	_isGameOver = false;

	_hpBar = new ProgressBar;
	_hpBar->init(65, 27, 644, 46);
	_hpBar->setBackImage(IMAGEMANAGER->findImage("ü�¹�����Ʈ"));

	_isDebug = true;

	loadMap(0.0f);

	RND->init();
	return S_OK;
}

void MainGame::release(void)
{
	GameNode::release();
	IMAGEMANAGER->release();
	_hurdleManager->release();
	SAFE_DELETE(_hurdleManager);
}

void MainGame::update(void)
{


	GameNode::update();

	if (_isGameOver)
	{
		// ���ӿ��� �ִϸ��̼�
		if (!_gameOverAnimationFinished)
		{
			if (_panCakeFrameCount++ % 10 == 0)
			{
				_panCakeFrameX++;
				if (_panCakeFrameX >= 4)
				{
					_panCakeFrameX = 4;
					_gameOverAnimationFinished = true;
				}
			}
		}

		// �� �̵� �ӵ��� 0���� ������ ����
		if (_mapPosX < 0.0f)
		{
			_mapPosX += 0.05f;
			if (_mapPosX > 0.0f) _mapPosX = 0.0f;
		}

		// ���ӵǴ� �ӵ��� ���� Ÿ��, ��ֹ�, ��� �̵�
		for (size_t i = 0; i < _tiles.size(); i++)
		{
			_tiles[i].left += (int)_mapPosX;
			_tiles[i].right += (int)_mapPosX;
		}
		_hurdleManager->update(_mapPosX);

		// ��� �̵� ���� ����
		_bgX += -_mapPosX * 0.25f;
		_bgObj1X += -_mapPosX * 0.4f;
		_bgObj2X += -_mapPosX * 0.6f;
		_bgObj3X += -_mapPosX;

		return; // �ٸ� ��� ���� �ǳʶ�
	}

	// �÷��̾� ��Ʈ�ڽ�
	if (_playerState == PlayerState::SLIDING)
	{
		_playerHitbox = RectMakeCenter(_panCakeX + 100, (int)_panCakeY + 100, 100, 80);
	}
	else
	{
		_playerHitbox = RectMakeCenter(_panCakeX + 100, (int)_panCakeY + 76, 70, 130);

	}

	_mapPosX = -8.0f; // Ÿ�� �̵� �ӵ�

	for (size_t i = 0; i < _tiles.size(); )
	{
		_tiles[i].left += (int)_mapPosX;
		_tiles[i].right += (int)_mapPosX;

		// ȭ�� �������� ����� Ÿ���� ����
		if (_tiles[i].right < 0)
		{
			_tiles.erase(_tiles.begin() + i);
		}
		else i++;
	}

	// Ÿ�� ����
	if (!_tiles.empty() && _tiles.back().left < WINSIZE_X)
	{
		// ������ Ÿ���� ������ �� ��ǥ�� �̾ ���� ���� �ε��մϴ�.
		loadMap(_tiles.back().right);
	}
	_hurdleManager->update(_mapPosX);

	// Ű �Է� ó��

	if (_playerState != PlayerState::HIT)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		{
			if ((_playerState == PlayerState::RUNNING || _playerState == PlayerState::SLIDING || _playerState == PlayerState::LANDING))
			{
				_playerState = PlayerState::JUMPING;
				_velocityY = -_jumpPower;
				_canDoubleJump = true;
				_panCakeFrameX = 0;
				_panCakeFrameCount = 0;
			}
			else if (_playerState == PlayerState::JUMPING && _canDoubleJump)
			{
				_playerState = PlayerState::DOUBLE_JUMPING;
				_velocityY = -_jumpPower;
				_canDoubleJump = false;
				_panCakeFrameX = 0;
				_panCakeFrameCount = 0;
			}
		}
	}

	// ���� ȿ�� ����
	_velocityY += _gravity;
	_panCakeY += _velocityY;


	// �浹 ó��
	bool onGround = false;
	for (auto& tile : _tiles)
	{
		RECT playerFeet = RectMake(_playerHitbox.left + 10, _playerHitbox.bottom - 10, (_playerHitbox.right - _playerHitbox.left) - 20, 10);
		RECT intersection;
		// �÷��̾ �Ʒ��� �������� �߿��� Ÿ�ϰ� �浹 ����
		if (_velocityY > 0 && IntersectRect(&intersection, &playerFeet, &tile))
		{
			// �÷��̾ Ÿ�� �ٷ� ���� �̵�
			_panCakeY -= (_playerHitbox.bottom - tile.top);
			_velocityY = 0;
			onGround = true;

			// JUMPING �Ǵ� DOUBLE_JUMPING ���¿��ٸ� LANDING���� ����
			if ((_playerState == PlayerState::JUMPING || _playerState == PlayerState::DOUBLE_JUMPING))
			{
				_playerState = PlayerState::LANDING;
				_landingTimer = _landingTime;
			}
			else if (_playerState == PlayerState::LANDING && KEYMANAGER->isStayKeyDown(VK_SHIFT))
			{
				_playerState = PlayerState::SLIDING;

			}
			break;
		}
	}

	// �������� ���� ��ȭ (���� Ÿ�̸�, �����̵�)
	if (_playerState == PlayerState::LANDING)
	{
		_landingTimer -= 1.0f / 60.0f;
		if (_landingTimer <= 0)
		{
			_playerState = PlayerState::RUNNING;
		}
	}
	else if (onGround) // LANDING�� �ƴϸ鼭 ���� ���� ��
	{
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


	// �浹 ó��
	if (!_isInvincible)
	{
		for (auto& hurdle : _hurdleManager->getHurdles())
		{
			RECT temp;
			if (IntersectRect(&temp, &_playerHitbox, &hurdle->getRect()))
			{
				_currentHp -= 20.0f;
				_playerState = PlayerState::HIT;
				_isInvincible = true;
				_invincibleTimer = 2.0f; // 2�� ����
				_panCakeFrameX = 0;
				_panCakeFrameCount = 0;
				_hitAnimationFinished = false;
				break;
			}
		}
	}

	if (_isInvincible)
	{
		_invincibleTimer -= 1.0f / 30.0f;
		if (_invincibleTimer <= 0)
		{
			_isInvincible = false;

			if (!_isGameOver)
			{
				_playerState = PlayerState::RUNNING;
			}
		}
	}

	// ü�� ���ҷ�
	_currentHp -= 0.017f;

	// ü���� 0 ���ϰ� �Ǹ� ���ӿ���
	if (_currentHp <= 0)
	{
		_currentHp = 0;
		_isGameOver = true;
		_playerState = PlayerState::GAMEOVER;
		_panCakeFrameX = 0;
		_panCakeFrameCount = 0;
	}

	// HP �� ������Ʈ
	_hpBar->setGauge(_currentHp, _maxHp);
	_hpBar->update();



	// �޹�� �ӵ�ó��
	_bgX += 2.0f;
	_bgObj1X += 3.0f;
	_bgObj2X += 4.0f;
	_bgObj3X += 8.0f;


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
	case PlayerState::HIT:
		if (!_hitAnimationFinished)
		{
			if (_panCakeFrameCount % 5 == 0)
			{
				_panCakeFrameX++;
				// ������ �����ӿ� �����ϸ�
				if (_panCakeFrameX >= 5)
				{
					_panCakeFrameX = 5; // ������ ���������� ����
					_hitAnimationFinished = true; // �ִϸ��̼� ���� �÷��� ����
					_playerState = PlayerState::RUNNING; // ��� �޸��� ���·� ��ȯ�Ͽ� ���� �����ϰ� ��
				}
			}
		}
		break;
	}
}

void MainGame::render(HDC hdc)
{
	HDC memDC = this->getBackBuffer()->getMemDC();
	PatBlt(memDC, 0, 0, WINSIZE_X, WINSIZE_Y, WHITENESS);
	// ============

	RECT rc = RectMake(0, 0, WINSIZE_X, WINSIZE_Y);



	IMAGEMANAGER->findImage("���")->loopRender(memDC, &rc, (int)_bgX, 0);
	IMAGEMANAGER->findImage("�����1")->loopRender(memDC, &rc, (int)_bgObj1X, 0);
	IMAGEMANAGER->findImage("�����2")->loopRender(memDC, &rc, (int)_bgObj2X, 0);
	IMAGEMANAGER->findImage("�����3")->loopRender(memDC, &rc, (int)_bgObj3X, 0);

	for (auto& tile : _tiles)
	{
		IMAGEMANAGER->findImage("Ÿ��")->render(memDC, tile.left, tile.top);
	}

	_hurdleManager->render(memDC);


	int renderY = (int)_panCakeY;
	if (_playerState != PlayerState::GAMEOVER && _isInvincible && (int)(_invincibleTimer * 10) % 2 == 0) // ������ �ڵ�
	{

	}
	else
	{
		switch (_playerState)
		{
		case PlayerState::RUNNING:
			IMAGEMANAGER->frameRender("�⺻�޸���", memDC, _panCakeX, renderY, _panCakeFrameX, 0);
			break;
		case PlayerState::SLIDING:
			IMAGEMANAGER->frameRender("�⺻�����̵�", memDC, _panCakeX, renderY + (144 - 108), _panCakeFrameX, 0);
			break;
		case PlayerState::JUMPING:
			IMAGEMANAGER->frameRender("����", memDC, _panCakeX, renderY + (144 - 140), _panCakeFrameX, 0);
			break;
		case PlayerState::DOUBLE_JUMPING:
			IMAGEMANAGER->frameRender("��������", memDC, _panCakeX, renderY + (144 - 146), _panCakeFrameX, 0);
			break;
		case PlayerState::LANDING:
			IMAGEMANAGER->frameRender("����", memDC, _panCakeX, renderY + (144 - 126), _panCakeFrameX, 0);
			break;
		case PlayerState::HIT:
			IMAGEMANAGER->frameRender("�浹", memDC, _panCakeX, renderY + (144 - 158), _panCakeFrameX, 0);
			break;
		case PlayerState::GAMEOVER:
			IMAGEMANAGER->frameRender("���ӿ���", memDC, _panCakeX, renderY, _panCakeFrameX, 0);
			break;
		}
	}

	_hpBar->render(memDC);
	IMAGEMANAGER->findImage("ü�¾�����")->render(memDC, 20, 10);

	if (_isDebug)
	{
		HPEN myPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
		HPEN oldPen = (HPEN)SelectObject(memDC, myPen);

		// ������ �귯�� ����
		HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		HBRUSH oldBrush = (HBRUSH)SelectObject(memDC, myBrush);

		// �÷��̾� ��Ʈ�ڽ� �׸���
		DrawRectMake(memDC, _playerHitbox);

		// Ÿ�� ��Ʈ�ڽ� �׸���
		for (auto& tile : _tiles)
		{
			DrawRectMake(memDC, tile);
		}
		for (auto& hurdle : _hurdleManager->getHurdles())
		{
			DrawRectMake(memDC, hurdle->getRect());
		}
		SelectObject(memDC, oldPen);
		SelectObject(memDC, oldBrush);
		DeleteObject(myPen);
	}


	// ============
	this->getBackBuffer()->render(hdc, 0, 0);
}

void MainGame::loadMap(float startX)
{
	std::string mapData = "TTTTTTTTTSSSTTTTTTLTTLTTLTTTTTTTTTTTTTTSTTLTTSTTLTTSTTLTTTTTTTTHTTTTTTTTTTTTHTTTTTT";

	const int TILE_WIDTH = 129;
	const int TILE_HEIGHT = 50;
	const int GROUND_Y = WINSIZE_Y - 100;

	for (size_t i = 0; i < mapData.length(); ++i)
	{
		// T(Ÿ��), L(���� ���), H(���� ���)�� ��� Ÿ���� ����
		int currentX = startX + (i * TILE_WIDTH);
		char objectType = mapData[i];

		if (objectType == 'T' || objectType == 'L' || objectType == 'H' || objectType == 'S')
		{
			_tiles.push_back(RectMake(currentX, GROUND_Y, TILE_WIDTH, TILE_HEIGHT));
			if (objectType == 'L')
			{
				_hurdleManager->createHurdle(HurdleType::LOW, currentX, GROUND_Y);
			}
			else if (objectType == 'H')
			{
				_hurdleManager->createHurdle(HurdleType::HIGH, currentX, GROUND_Y);
			}
			else if (objectType == 'S')
			{
				_hurdleManager->createHurdle(HurdleType::SPIKE, currentX, 0);
			}
		}
	}
}