#include "Stdafx.h"
#include "MainGame.h"

// 멤버 변수 초기화
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

	// 배경 + 배경요소
	IMAGEMANAGER->addImage("배경", "Resources/Images/BackGround/BackGround.bmp", WINSIZE_X, WINSIZE_Y);
	IMAGEMANAGER->addImage("배경요소1", "Resources/Images/BackGround/BackGroundObject1.bmp", WINSIZE_X, WINSIZE_Y, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("배경요소2", "Resources/Images/BackGround/BackGroundObject2.bmp", WINSIZE_X, WINSIZE_Y, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("배경요소3", "Resources/Images/BackGround/BackGroundObject3.bmp", WINSIZE_X, WINSIZE_Y, true, RGB(255, 0, 255));
	

	// 애니메이션
	IMAGEMANAGER->addFrameImage("기본달리기", "Resources/Images/Object/PanCakeRun.bmp", 181 * 4, 144, 4, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("기본슬라이드", "Resources/Images/Object/PanCakeSlide.bmp", 324, 108, 2, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("점프", "Resources/Images/Object/PanCakeJump.bmp", 334, 140, 2, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("더블점프", "Resources/Images/Object/PanCakeDoubleJump.bmp", 1062, 146, 6, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("착지", "Resources/Images/Object/PanCakeLanding.bmp", 150, 126, 1, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("충돌", "Resources/Images/Object/PanCakeCollision.bmp", 1200, 158, 6, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("게임오버", "Resources/Images/Object/PanCakeGameOver.bmp", 915, 146, 5, 1, true, RGB(255, 0, 255));

	// 장애물
	IMAGEMANAGER->addFrameImage("허들준비", "Resources/Images/Object/hurdleReady.bmp", 138, 131, 2, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("낮은허들", "Resources/Images/Object/hurdle.bmp", 388, 148, 4, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("높은허들", "Resources/Images/Object/highHurdle.bmp", 475, 296, 5, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("스파이크", "Resources/Images/Object/spike2.bmp", 137, 416, true, RGB(255, 0, 255));


	// 타일
	IMAGEMANAGER->addImage("타일", "Resources/Images/Object/tile.bmp", 129, 50, true, RGB(255, 0, 255));

	// UI
	IMAGEMANAGER->addImage("체력아이콘", "Resources/Images/UI/iconHeartLife.bmp", 54, 54, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("체력바배경", "Resources/Images/UI/gaugebgHeart.bmp", 650, 25, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("체력바", "Resources/Images/UI/gaugeHeartOrange.bmp", 644, 23, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("체력바이펙트", "Resources/Images/UI/gaueHeartEffect.bmp", 33, 30, 2, 1, true, RGB(255, 0, 255));


	_panCakeX = 130;
	//_groundY = WINSIZE_Y - 250;
	_panCakeY = WINSIZE_Y - 250;;
	_bgX = 0.0f;
	_bgObj1X = 0.0f;
	_bgObj2X = 0.0f;
	_bgObj3X = 0.0f;

	// 물리 변수
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
	_hpBar->setBackImage(IMAGEMANAGER->findImage("체력바이펙트"));

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
		// 게임오버 애니메이션
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

		// 맵 이동 속도를 0으로 서서히 감소
		if (_mapPosX < 0.0f)
		{
			_mapPosX += 0.05f;
			if (_mapPosX > 0.0f) _mapPosX = 0.0f;
		}

		// 감속되는 속도에 맞춰 타일, 장애물, 배경 이동
		for (size_t i = 0; i < _tiles.size(); i++)
		{
			_tiles[i].left += (int)_mapPosX;
			_tiles[i].right += (int)_mapPosX;
		}
		_hurdleManager->update(_mapPosX);

		// 배경 이동 방향 수정
		_bgX += -_mapPosX * 0.25f;
		_bgObj1X += -_mapPosX * 0.4f;
		_bgObj2X += -_mapPosX * 0.6f;
		_bgObj3X += -_mapPosX;

		return; // 다른 모든 로직 건너뜀
	}

	// 플레이어 히트박스
	if (_playerState == PlayerState::SLIDING)
	{
		_playerHitbox = RectMakeCenter(_panCakeX + 100, (int)_panCakeY + 100, 100, 80);
	}
	else
	{
		_playerHitbox = RectMakeCenter(_panCakeX + 100, (int)_panCakeY + 76, 70, 130);

	}

	_mapPosX = -8.0f; // 타일 이동 속도

	for (size_t i = 0; i < _tiles.size(); )
	{
		_tiles[i].left += (int)_mapPosX;
		_tiles[i].right += (int)_mapPosX;

		// 화면 왼쪽으로 사라진 타일은 제거
		if (_tiles[i].right < 0)
		{
			_tiles.erase(_tiles.begin() + i);
		}
		else i++;
	}

	// 타일 생성
	if (!_tiles.empty() && _tiles.back().left < WINSIZE_X)
	{
		// 마지막 타일의 오른쪽 끝 좌표에 이어서 다음 맵을 로드합니다.
		loadMap(_tiles.back().right);
	}
	_hurdleManager->update(_mapPosX);

	// 키 입력 처리

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

	// 물리 효과 적용
	_velocityY += _gravity;
	_panCakeY += _velocityY;


	// 충돌 처리
	bool onGround = false;
	for (auto& tile : _tiles)
	{
		RECT playerFeet = RectMake(_playerHitbox.left + 10, _playerHitbox.bottom - 10, (_playerHitbox.right - _playerHitbox.left) - 20, 10);
		RECT intersection;
		// 플레이어가 아래로 떨어지는 중에만 타일과 충돌 판정
		if (_velocityY > 0 && IntersectRect(&intersection, &playerFeet, &tile))
		{
			// 플레이어를 타일 바로 위로 이동
			_panCakeY -= (_playerHitbox.bottom - tile.top);
			_velocityY = 0;
			onGround = true;

			// JUMPING 또는 DOUBLE_JUMPING 상태였다면 LANDING으로 변경
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

	// 땅에서의 상태 변화 (착지 타이머, 슬라이드)
	if (_playerState == PlayerState::LANDING)
	{
		_landingTimer -= 1.0f / 60.0f;
		if (_landingTimer <= 0)
		{
			_playerState = PlayerState::RUNNING;
		}
	}
	else if (onGround) // LANDING이 아니면서 땅에 있을 때
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


	// 충돌 처리
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
				_invincibleTimer = 2.0f; // 2초 무적
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

	// 체력 감소량
	_currentHp -= 0.017f;

	// 체력이 0 이하가 되면 게임오버
	if (_currentHp <= 0)
	{
		_currentHp = 0;
		_isGameOver = true;
		_playerState = PlayerState::GAMEOVER;
		_panCakeFrameX = 0;
		_panCakeFrameCount = 0;
	}

	// HP 바 업데이트
	_hpBar->setGauge(_currentHp, _maxHp);
	_hpBar->update();



	// 뒷배경 속도처리
	_bgX += 2.0f;
	_bgObj1X += 3.0f;
	_bgObj2X += 4.0f;
	_bgObj3X += 8.0f;


	// 각 애니메이션 별 프레임 속도 처리
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
				// 마지막 프레임에 도달하면
				if (_panCakeFrameX >= 5)
				{
					_panCakeFrameX = 5; // 마지막 프레임으로 고정
					_hitAnimationFinished = true; // 애니메이션 종료 플래그 설정
					_playerState = PlayerState::RUNNING; // 즉시 달리기 상태로 전환하여 조작 가능하게 함
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



	IMAGEMANAGER->findImage("배경")->loopRender(memDC, &rc, (int)_bgX, 0);
	IMAGEMANAGER->findImage("배경요소1")->loopRender(memDC, &rc, (int)_bgObj1X, 0);
	IMAGEMANAGER->findImage("배경요소2")->loopRender(memDC, &rc, (int)_bgObj2X, 0);
	IMAGEMANAGER->findImage("배경요소3")->loopRender(memDC, &rc, (int)_bgObj3X, 0);

	for (auto& tile : _tiles)
	{
		IMAGEMANAGER->findImage("타일")->render(memDC, tile.left, tile.top);
	}

	_hurdleManager->render(memDC);


	int renderY = (int)_panCakeY;
	if (_playerState != PlayerState::GAMEOVER && _isInvincible && (int)(_invincibleTimer * 10) % 2 == 0) // 수정된 코드
	{

	}
	else
	{
		switch (_playerState)
		{
		case PlayerState::RUNNING:
			IMAGEMANAGER->frameRender("기본달리기", memDC, _panCakeX, renderY, _panCakeFrameX, 0);
			break;
		case PlayerState::SLIDING:
			IMAGEMANAGER->frameRender("기본슬라이드", memDC, _panCakeX, renderY + (144 - 108), _panCakeFrameX, 0);
			break;
		case PlayerState::JUMPING:
			IMAGEMANAGER->frameRender("점프", memDC, _panCakeX, renderY + (144 - 140), _panCakeFrameX, 0);
			break;
		case PlayerState::DOUBLE_JUMPING:
			IMAGEMANAGER->frameRender("더블점프", memDC, _panCakeX, renderY + (144 - 146), _panCakeFrameX, 0);
			break;
		case PlayerState::LANDING:
			IMAGEMANAGER->frameRender("착지", memDC, _panCakeX, renderY + (144 - 126), _panCakeFrameX, 0);
			break;
		case PlayerState::HIT:
			IMAGEMANAGER->frameRender("충돌", memDC, _panCakeX, renderY + (144 - 158), _panCakeFrameX, 0);
			break;
		case PlayerState::GAMEOVER:
			IMAGEMANAGER->frameRender("게임오버", memDC, _panCakeX, renderY, _panCakeFrameX, 0);
			break;
		}
	}

	_hpBar->render(memDC);
	IMAGEMANAGER->findImage("체력아이콘")->render(memDC, 20, 10);

	if (_isDebug)
	{
		HPEN myPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
		HPEN oldPen = (HPEN)SelectObject(memDC, myPen);

		// 투명한 브러쉬 선택
		HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		HBRUSH oldBrush = (HBRUSH)SelectObject(memDC, myBrush);

		// 플레이어 히트박스 그리기
		DrawRectMake(memDC, _playerHitbox);

		// 타일 히트박스 그리기
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
		// T(타일), L(낮은 허들), H(높은 허들)일 경우 타일을 생성
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