#include "Stdafx.h"
#include "MainGame.h"
#include "ItemManager.h"
#include "EffectManager.h"


// 멤버 변수 초기화
MainGame::MainGame() :
	_panCakeX(130),
	_panCakeY(WINSIZE_Y - 250),
	_panCakeFrameX(0),
	_panCakeFrameCount(0),
	_playerHitbox(RectMake(0, 0, 0, 0)),
	_playerState(PlayerState::RUNNING),
	_canDoubleJump(false),
	_hitAnimationFinished(false),
	_gameOverAnimationFinished(false),
	_isSprinting(false),
	_isInvincible(false),
	_isPostSprintInvincible(false),
	_jumpPower(20.0f),					// 점프 힘
	_gravity(0.97f),					// 중력값
	_velocityY(0.0f),
	_landingTime(0.1f),					// 착지 애니메이션
	_landingTimer(0.0f),
	_sprintTimer(0.0f),
	_originalMapSpeed(-8.0f),			// 오브젝트 기본 이동 속도
	_invincibleTimer(0.0f),
	_postSprintInvincibleTimer(0.0f),
	_isMagnetActive(false),
	_magnetTimer(0.0f),
	// BG
	_bgX(0.0f),
	_bgObj1X(0.0f),
	_bgObj2X(0.0f),
	_bgObj3X(0.0f),
	_mapPosX(0.0f),
	// Object Managers (Pointers)
	_itemManager(nullptr),
	_hurdleManager(nullptr),
	_effectManager(nullptr),
	// UI & etc.
	_hpBar(nullptr),
	_currentHp(100.0f),					// 현재HP
	_maxHp(100.0f),						// 최대HP
	_damageAlpha(0.0f),
	_isGameOver(false),
	_isShowingDamage(false),
	_isDebug(true),
	//Ablity
	_abilityGaugeTimer(0.0f),
	_abilityChargeTime(15.0f),			// 능력 충전 시간 
	_isAbilityActive(false),
	_abilityDurationTimer(0.0f),
	_abilityDuration(7.0f),				// 능력 지속 시간
	_postAbilityInvincibleTimer(0.0f),
	_flyingYSpeed(5.5f),				// 나는 속도
	_flyingState(FlyingState::DOWN),
	_flyingFrameX(0),
	_flyingFrameCount(0),
	_transformationDuration(0.5f),		// 변신 시간
	_transformationTimer(0.0f),
	_flyingTransitionDuration(0.07f),	// 중간 애니메이션
	_flyingTransitionTimer(0.0f),
	_magnetEffectFrameX(0),
	_magnetEffectFrameCount(0)
{
}

// ====================================================================================================
// ============================================== INIT ==============================================
// ====================================================================================================

HRESULT MainGame::init(void)
{
	GameNode::init();
	IMAGEMANAGER->init();
	EFFECTMANAGER->init();
	SOUNDMANAGER->init();

	_effectManager = EFFECTMANAGER;

	_hurdleManager = new HurdleManager();
	_hurdleManager->init();

	_itemManager = new ItemManager();
	_itemManager->init();

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
	IMAGEMANAGER->addFrameImage("질주", "Resources/Images/Object/PanCakeSprint.bmp", 668, 149, 4, 1, true, RGB(255, 0, 255));


	// 장애물
	IMAGEMANAGER->addFrameImage("허들준비", "Resources/Images/Object/hurdleReady.bmp", 138, 131, 2, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("낮은허들", "Resources/Images/Object/hurdle.bmp", 388, 148, 4, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("높은허들", "Resources/Images/Object/highHurdle.bmp", 475, 296, 5, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("스파이크", "Resources/Images/Object/spike2.bmp", 137, 416, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("장애물파괴", "Resources/Images/Effect/explode.bmp", 1568, 226, 7, 1, true, RGB(255, 0, 255));


	// 타일
	IMAGEMANAGER->addImage("타일", "Resources/Images/Object/tile.bmp", 129, 50, true, RGB(255, 0, 255));

	// UI
	IMAGEMANAGER->addImage("체력아이콘", "Resources/Images/UI/iconHeartLife.bmp", 54, 54, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("체력바배경", "Resources/Images/UI/gaugebgHeart.bmp", 650, 25, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("체력바", "Resources/Images/UI/gaugeHeartOrange.bmp", 644, 23, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("체력바이펙트", "Resources/Images/UI/gaueHeartEffect.bmp", 33, 30, 2, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("데미지", "Resources/Images/UI/damage.bmp", WINSIZE_X, WINSIZE_Y, true, RGB(255, 0, 255));

	// 젤리
	IMAGEMANAGER->addImage("젤리", "Resources/Images/Object/jelly.bmp", 25, 34, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("젤리이펙트", "Resources/Images/Effect/jellyEffect.bmp", 86, 46, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("곰젤리이펙트", "Resources/Images/Effect/bearJellyEffect.bmp", 96, 56, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("곰젤리", "Resources/Images/Object/bearJelly.bmp", 55, 51, true, RGB(255, 0, 255));

	// 아이템
	IMAGEMANAGER->addFrameImage("질주아이템", "Resources/Images/Object/booster.bmp", 240, 60, 4, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("자석아이템", "Resources/Images/Object/magnetic.bmp", 240, 60, 4, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("아이템이펙트", "Resources/Images/Effect/itemEaten.bmp", 272, 68, 4, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("질주이펙트", "Resources/Images/Effect/sprint.bmp", 151, 141, 1, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("자석이펙트", "Resources/Images/Effect/mageticEffect.bmp", 1452, 216, 6, 1, true, RGB(255, 0, 255));

	// 쿠키 능력
	IMAGEMANAGER->addFrameImage("비행상승", "Resources/Images/Object/PanCakeFlyingUp.bmp", 426, 158, 2, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("비행중간", "Resources/Images/Object/PanCakeFlyingMiddle.bmp", 213, 158, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("비행하강", "Resources/Images/Object/PanCakeFlyingDown.bmp", 426, 158, 2, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("능력게이지Empty", "Resources/Images/UI/ablityEmpty.bmp", 66, 21, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("능력게이지Full", "Resources/Images/UI/ablityFull.bmp", 66, 21, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("변신애니메이션", "Resources/Images/Object/panCakeTransform.bmp", 1750, 155, 10, 1, true, RGB(255, 0, 255));
	
	// 변수 초기 설정
	_panCakeX = 130;
	//_groundY = WINSIZE_Y - 250;
	_panCakeY = WINSIZE_Y - 250;;
	_bgX = 0.0f;
	_bgObj1X = 0.0f;
	_bgObj2X = 0.0f;
	_bgObj3X = 0.0f;

	_playerState = PlayerState::RUNNING;
	_jumpPower = 20.0f;
	_gravity = 0.97f;
	_velocityY = 0.0f;
	_landingTime = 0.1f;
	_landingTimer = 0.0f;
	_panCakeFrameX = 0;
	_panCakeFrameCount = 0;
	_invincibleTimer = 0.0f;
	_postSprintInvincibleTimer = 0;
	_mapPosX = 0;
	_maxHp = 100.0f;
	_currentHp = _maxHp;
	_canDoubleJump = false;
	_isInvincible = false;
	_hitAnimationFinished = false;
	_gameOverAnimationFinished = false;
	_isGameOver = false;
	_isSprinting = false;
	_isPostSprintInvincible = false;
	_isDebug = true;
	_hpBar = new ProgressBar;
	_hpBar->init(65, 27, 644, 46);
	_hpBar->setBackImage(IMAGEMANAGER->findImage("체력바이펙트"));
	_sprintTimer = 0.0f;
	_originalMapSpeed = -8.0f;

	loadMap(0.0f);

	SOUNDMANAGER->playBGM("BGM");

	RND->init();
	return S_OK;
}

// ====================================================================================================
// ============================================== RELEASE ==============================================
// ====================================================================================================

void MainGame::release(void)
{
	GameNode::release();
	IMAGEMANAGER->release();
	_hurdleManager->release();
	SAFE_DELETE(_hurdleManager);
	_itemManager->release();
	SAFE_DELETE(_itemManager);
	EFFECTMANAGER->releaseSingleton();
	SOUNDMANAGER->releaseSingleton();
}

// ====================================================================================================
// ============================================== UPDATE ==============================================
// ====================================================================================================

void MainGame::update(void)
{
	GameNode::update();

	
	// 게임 오버 조건
	if ((_currentHp <= 0 || _panCakeY > WINSIZE_Y + 300) && !_isGameOver)
	{
		if (_panCakeY > WINSIZE_Y + 300) {
			_isShowingDamage = true;
			_damageAlpha = 80.0f;
			_currentHp = 0;
		}
		_isGameOver = true;
		_playerState = PlayerState::GAMEOVER;
		_panCakeFrameX = 0;
		_panCakeFrameCount = 0;
		_isInvincible = true; // 게임 오버 시 더 이상 피격되지 않도록
		_invincibleTimer = 9999.0f;
	}
	// 게임 오버 시 로직
	if (_isGameOver)
	{
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

		bool onGround = false;
		for (auto& tile : _tiles)
		{
			RECT playerFeet = RectMake(_playerHitbox.left + 10, _playerHitbox.bottom - 10, (_playerHitbox.right - _playerHitbox.left) - 20, 10);
			RECT intersection;
			if (_velocityY > 0 && IntersectRect(&intersection, &playerFeet, &tile))
			{
				_panCakeY -= (_playerHitbox.bottom - tile.top);
				_velocityY = 0;
				onGround = true;
				break;
			}
		}
		if (!onGround)
		{
			_velocityY += _gravity;
			_panCakeY += _velocityY;
			_playerHitbox = RectMakeCenter(_panCakeX + 100, (int)_panCakeY + 76, 70, 130);
		}
		if (_mapPosX < 0.0f)
		{
			_mapPosX += 0.3f;
			if (_mapPosX > 0.0f) _mapPosX = 0.0f;
			
			_effectManager->update(_mapPosX);
			_hurdleManager->update(_mapPosX);
			_itemManager->update(_mapPosX);

			for (size_t i = 0; i < _tiles.size(); i++)
			{
				_tiles[i].left += (int)_mapPosX;
				_tiles[i].right += (int)_mapPosX;
			}
			_bgX -= _mapPosX * 0.25f;
			_bgObj1X -= _mapPosX * 0.4f;
			_bgObj2X -= _mapPosX * 0.6f;
			_bgObj3X -= _mapPosX * 0.8f;
		}

		if (_isShowingDamage)
		{
			_damageAlpha -= 3.0f;
			if (_damageAlpha <= 0)
			{
				_damageAlpha = 0;
				_isShowingDamage = false;
			}
		}

		return;
	}

	if (_playerState == PlayerState::TRANSFORMING)
	{
		_transformationTimer -= 1.0f / 60.0f;
		if (_transformationTimer <= 0)
		{
			_playerState = PlayerState::FLYING; // 변신이 끝나면 비행 상태로 전환
			_isAbilityActive = true; // 실제 능력 효과는 이때부터 적용
		}
	}

	updateAbility();


	// 질주 이펙트
	if (_isSprinting && !_isAbilityActive)
	{
		_sprintTimer -= 1.0f / 60.0f;
		// 일정 시간마다 질주 이펙트 생성
		if (_panCakeFrameCount % 2 == 0)
		{
			_effectManager->createEffect("질주이펙트", _panCakeX - 30, (int)_panCakeY + 20, false);
		}

		if (_sprintTimer <= 0)
		{
			_isSprinting = false;
			_playerState = PlayerState::RUNNING;

			_isPostSprintInvincible = true;
			_postSprintInvincibleTimer = 2.0f;
		}
	}

	_effectManager->update(_mapPosX);
	_itemManager->update(_mapPosX);
	_itemManager->checkCollision(_playerHitbox);
	_hurdleManager->update(_mapPosX);

	// 자석 효과
	if (_isMagnetActive)
	{
		_magnetTimer -= 1.0f / 60.0f;
		_itemManager->updateMagnetEffect(_playerHitbox);

		_magnetEffectFrameCount++;
		if (_magnetEffectFrameCount % 5 == 0)
		{
			_magnetEffectFrameX = (_magnetEffectFrameX + 1) % 6;
		}

		if (_magnetTimer <= 0)
		{
			_isMagnetActive = false;
		}
	}

	// 질주 끝난 뒤 무적시간
	if (_isPostSprintInvincible)
	{
		_postSprintInvincibleTimer -= 1.0f / 60.0f;
		if (_postSprintInvincibleTimer <= 0)
		{
			_isPostSprintInvincible = false;
		}
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

	// 맵 이동 속도
	float speedMultiplier = 1.0f;
	if (_isSprinting || _isAbilityActive)
	{
		speedMultiplier = 2.0f;
	}
	_mapPosX = _originalMapSpeed * speedMultiplier;

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
		loadMap(_tiles.back().right);
	}
	

	// 키 입력 처리
	if (_playerState != PlayerState::HIT && _playerState != PlayerState::FLYING)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		{
			if ((_playerState == PlayerState::RUNNING || _playerState == PlayerState::SLIDING || _playerState == PlayerState::LANDING || _playerState == PlayerState::SPRINTING))
			{
				_playerState = PlayerState::JUMPING;
				_velocityY = -_jumpPower;
				_canDoubleJump = true;
				_panCakeFrameX = 0;
				_panCakeFrameCount = 0;

				SOUNDMANAGER->playSound("Jump1");
			}
			else if (_playerState == PlayerState::JUMPING && _canDoubleJump)
			{
				_playerState = PlayerState::DOUBLE_JUMPING;
				_velocityY = -_jumpPower;
				_canDoubleJump = false;
				_panCakeFrameX = 0;
				_panCakeFrameCount = 0;

				SOUNDMANAGER->playSound("Jump2");
			}
		}
	}

	// 물리 효과 적용
	if (!_isAbilityActive)
	{
		_velocityY += _gravity;
	}
	_panCakeY += _velocityY;

	if (_playerState == PlayerState::FLYING)
	{
		updateFlying();
	}

	bool onGround = false;

	// 타일 - 플레이어 충돌 처리
	if (_playerState != PlayerState::FLYING)
	{
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
	}

	// 플레이어 - 아이템 출동 처리
	for (auto& item:_itemManager->getItems())
	{
		if (!item->isEaten())
		{
			RECT temp;
			if (IntersectRect(&temp, &_playerHitbox, &item->getRect()))
			{
				if (item->getType() == ItemType::ITEM_SPRINT)
				{
					if (item->getType() == ItemType::ITEM_SPRINT && !_isAbilityActive)
					{
						item->eat();
						_isSprinting = true;
						_sprintTimer = 5.0f;
						_playerState = PlayerState::SPRINTING;
						_panCakeFrameX = 0;
						_panCakeFrameCount = 0;
					}
					else
					{
						item->eat();
					}
				}
				else if (item->getType() == ItemType::ITEM_MAGNET)
				{
					item->eat();
					_isMagnetActive = true;
					_magnetTimer = 7.0f;
				}
				else if (item->getType() == ItemType::JELLY_NORMAL || item->getType() == ItemType::JELLY_BEAR)
				{
					item->eat();
				}
			}
		}
	}

	// 착지, 슬라이드 처리
	if (_playerState == PlayerState::LANDING)
	{
		_landingTimer -= 1.0f / 60.0f;
		if (_landingTimer <= 0)
		{
			if (_isSprinting && !_isAbilityActive)
			{
				_playerState = PlayerState::SPRINTING;
			}
			else
			{
				_playerState = PlayerState::RUNNING;
			}
		}
	}
	else if (onGround)
	{
		if (KEYMANAGER->isStayKeyDown(VK_SHIFT))
		{
			if (_playerState == PlayerState::RUNNING || _playerState == PlayerState::SPRINTING)
			{
				_playerState = PlayerState::SLIDING;
				_panCakeFrameX = 0;
				_panCakeFrameCount = 0;

				if (RND->getInt(2) == 0) // 0 또는 1을 랜덤으로 생성
				{
					SOUNDMANAGER->playSound("Slide1");
				}
				else
				{
					SOUNDMANAGER->playSound("Slide2");
				}
			}
		}
		else if (_playerState == PlayerState::SLIDING)
		{
			if (_isSprinting && !_isAbilityActive)
			{
				_playerState = PlayerState::SPRINTING;
				_panCakeFrameX = 0;
				_panCakeFrameCount = 0;
			}
			else
			{
				_playerState = PlayerState::RUNNING;
				_panCakeFrameX = 0;
				_panCakeFrameCount = 0;
			}
		}
	}


	// 충돌 처리
	if (_isSprinting || _isAbilityActive)
	{
		for (auto& hurdle : _hurdleManager->getHurdles())
		{
			if (!hurdle->isDestroyed())
			{
				RECT temp;
				if (IntersectRect(&temp, &_playerHitbox, &hurdle->getRect()))
				{
					hurdle->destroy();
					// 파괴 위치에 이펙트 생성
					if (hurdle->getType() == HurdleType::SPIKE)
					{
						// Spike 장애물일 경우: 더 낮은 위치에 이펙트 생성
						_effectManager->createEffect("장애물파괴", hurdle->getRect().left - 50, hurdle->getRect().top + 150, true);
					}
					else
					{
						// 그 외 장애물일 경우: 기존 위치에 이펙트 생성
						_effectManager->createEffect("장애물파괴", hurdle->getRect().left - 50, hurdle->getRect().top - 50, true);
					}
				}
			}
		}
	}
	else if (!_isInvincible && !_isPostSprintInvincible && _playerState != PlayerState::TRANSFORMING)
	{
		for (auto& hurdle : _hurdleManager->getHurdles())
		{
			RECT temp;
			if (IntersectRect(&temp, &_playerHitbox, &hurdle->getRect()))
			{
				_currentHp -= 20.0f;
				_playerState = PlayerState::HIT;
				_isInvincible = true;
				_invincibleTimer = 2.5f; // 무적
				_panCakeFrameX = 0;
				_panCakeFrameCount = 0;
				_hitAnimationFinished = false;

				_isShowingDamage = true;
				_damageAlpha = 80.0f;
				break;
			}
		}
	}

	// 무적 타이머
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
	if (_currentHp <= 0 && !_isGameOver)
	{
		_currentHp = 0;
		_isGameOver = true;
		_playerState = PlayerState::GAMEOVER; 
		_panCakeFrameX = 0;
		_panCakeFrameCount = 0;

		_isInvincible = true;
		_invincibleTimer = 9999.0f;
	}

	// 바닥에 추락 시 게임오버
	if (!_isGameOver && _panCakeY > WINSIZE_Y + 300)
	{
		_isShowingDamage = true;
		_damageAlpha = 80.0f;
		_currentHp = 0;
		_isGameOver = true;
	}

	// 데미지 이펙트
	if (_isShowingDamage)
	{
		_damageAlpha -= 3.0f; // 투명해지는 속도 (값 조절 가능)
		if (_damageAlpha <= 0)
		{
			_damageAlpha = 0;
			_isShowingDamage = false;
		}
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
	case PlayerState::SPRINTING:
		if (_panCakeFrameCount % 5 == 0) _panCakeFrameX = (_panCakeFrameX + 1) % 4;
		break;
	case PlayerState::FLYING:
		break;
	case PlayerState::TRANSFORMING:
		if (_panCakeFrameCount % 5 == 0) _panCakeFrameX = (_panCakeFrameX + 1) % 4;
		break;
	}
}

// ====================================================================================================
// ============================================== RENDER ==============================================
// ====================================================================================================

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
	_itemManager->render(memDC);
	_effectManager->render(memDC);

	// 자석 이펙트
	if (_isMagnetActive)
	{
		GImage* magnetEffectImg = IMAGEMANAGER->findImage("자석이펙트");
		if (magnetEffectImg)
		{
			int effectX = _playerHitbox.left + (_playerHitbox.right - _playerHitbox.left - magnetEffectImg->getFrameWidth()) / 2;
			int effectY = _playerHitbox.top + (_playerHitbox.bottom - _playerHitbox.top - magnetEffectImg->getFrameHeight()) / 2;

			magnetEffectImg->alphaFrameRender(memDC, effectX - 10, effectY, _magnetEffectFrameX, 0, 100);
		}
	}

	// 능력 게이지 render
	IMAGEMANAGER->findImage("능력게이지Empty")->render(memDC, _panCakeX + 60, _panCakeY - 20);
	float gaugeRatio = 0.0f;
	if (_isAbilityActive)
	{
		gaugeRatio = _abilityDurationTimer / _abilityDuration;
	}
	else
	{
		gaugeRatio = _abilityGaugeTimer / _abilityChargeTime;
	}

	if (gaugeRatio > 1.0f) gaugeRatio = 1.0f;
	if (gaugeRatio < 0.0f) gaugeRatio = 0.0f;

	GImage* fullGaugeImg = IMAGEMANAGER->findImage("능력게이지Full");
	if (fullGaugeImg)
	{
		int gaugeWidth = fullGaugeImg->getWidth() * gaugeRatio;
		fullGaugeImg->render(memDC, _panCakeX + 60, _panCakeY - 20, 0, 0, gaugeWidth, fullGaugeImg->getHeight());
	}

	int renderY = (int)_panCakeY;
	bool shouldRenderPlayer = true;

	// 피격 무적 상태일 때 깜빡임 처리
	if (_isInvincible && (int)(_invincibleTimer * 10) % 2 == 0)
	{
		shouldRenderPlayer = false;
	}

	// 질주 후 무적 상태일 때 깜빡임 처리
	if (_isPostSprintInvincible && (int)(_postSprintInvincibleTimer * 10) % 2 == 0)
	{
		shouldRenderPlayer = false;
	}

	// 게임오버 상태에서는 항상 보이도록
	if (_playerState == PlayerState::GAMEOVER)
	{
		shouldRenderPlayer = true;
	}

	if (shouldRenderPlayer)
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
		case PlayerState::SPRINTING:
			IMAGEMANAGER->frameRender("질주", memDC, _panCakeX, renderY, _panCakeFrameX, 0);
			break;
		case PlayerState::TRANSFORMING:
			IMAGEMANAGER->frameRender("변신애니메이션", memDC, _panCakeX, renderY, _panCakeFrameX, 0);
			break;
		case PlayerState::FLYING:
			switch (_flyingState)
			{
			case FlyingState::UP:
				IMAGEMANAGER->frameRender("비행상승", memDC, _panCakeX, renderY, _flyingFrameX, 0);
				break;
			case FlyingState::DOWN:
				IMAGEMANAGER->frameRender("비행하강", memDC, _panCakeX, renderY, _flyingFrameX, 0);
				break;
			case FlyingState::MIDDLE_TO_UP:
				IMAGEMANAGER->findImage("비행중간")->render(memDC, _panCakeX, renderY);
				break;
			case FlyingState::MIDDLE_TO_DOWN:
				IMAGEMANAGER->findImage("비행중간")->render(memDC, _panCakeX, renderY);
				break;
			default:
				break;
			}
		}
	}

	if (_isShowingDamage)
	{
		IMAGEMANAGER->findImage("데미지")->alphaRenderWithTransparency(memDC, 0, 0, static_cast<BYTE>(_damageAlpha));
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
	std::string jellyData = "-----TTTMSSSTTTTT-L--L--L-TBTTTTTTTTTTTST-L-TST-L-TST-L-TTTTTT-H-T-H-T-H-T-H-T-H-T-H-TTTBTT";
	std::string mapData = "TTT--TTTTSSSTTTTTTLTTLTTLTTTTTT--T--TTTSTTLTTSTTLTTSTTLTTTTTTTTHTTTHTTTHTTTHTTTHTTTHTTTTTTTHHHHHHHHHHHHHHHHHHHHH";

	const int TILE_WIDTH = 129;
	const int TILE_HEIGHT = 50;
	const int GROUND_Y = WINSIZE_Y - 100;

	std::vector<RECT> currentTiles;
	for (size_t i = 0; i < mapData.length(); ++i)
	{
		// T(타일), L(낮은 허들), H(높은 허들)일 경우 타일을 생성
		int currentX = startX + (i * TILE_WIDTH);
		char objectType = mapData[i];

		if (objectType == 'T' || objectType == 'L' || objectType == 'H' || objectType == 'S')
		{
			// _tiles.push_back(RectMake(currentX, GROUND_Y, TILE_WIDTH, TILE_HEIGHT));
			RECT tileRect = RectMake(currentX, GROUND_Y, TILE_WIDTH, TILE_HEIGHT);
			_tiles.push_back(tileRect);
			currentTiles.push_back(tileRect);

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
	_itemManager->createItems(jellyData, startX);
}

void MainGame::updateAbility()
{
	if (!_isAbilityActive && _playerState != PlayerState::TRANSFORMING)
	{
		_abilityGaugeTimer += 1.0f / 60.0f;
		if (_abilityGaugeTimer >= _abilityChargeTime)
		{
			_isSprinting = false;
			_sprintTimer = 0.0f;

			_playerState = PlayerState::TRANSFORMING;		
			_transformationTimer = _transformationDuration; 
			_abilityDurationTimer = _abilityDuration;		

			_velocityY = 0; 
			_panCakeFrameX = 0;
			_panCakeFrameCount = 0;
		}
	}
	// 능력이 활성 상태일 때
	else if(_isAbilityActive)
	{
		_abilityDurationTimer -= 1.0f / 60.0f; // 지속시간 감소
		// 지속시간이 끝나면
		if (_abilityDurationTimer <= 0)
		{
			_isAbilityActive = false;
			_abilityGaugeTimer = 0.0f; // 게이지 타이머 초기화
			_playerState = PlayerState::RUNNING;
			_isPostSprintInvincible = true; // 능력 종료 후 무적
			_postSprintInvincibleTimer = 2.0f; // 2초간 무적
		}
	}
}

void MainGame::updateFlying()
{
	FlyingState prevState = _flyingState;

	FlyingState targetState = KEYMANAGER->isStayKeyDown(VK_SPACE) ? FlyingState::UP : FlyingState::DOWN;

	if (_flyingState != targetState && _flyingState != FlyingState::MIDDLE_TO_UP && _flyingState != FlyingState::MIDDLE_TO_DOWN)
	{
		if (targetState == FlyingState::UP) _flyingState = FlyingState::MIDDLE_TO_UP;
		else _flyingState = FlyingState::MIDDLE_TO_DOWN;
		_flyingTransitionTimer = _flyingTransitionDuration; // 전환 타이머 설정
	}

	switch (_flyingState)
	{
	case FlyingState::MIDDLE_TO_UP:
		_flyingTransitionTimer -= 1.0f / 60.0f;
		if (_flyingTransitionTimer <= 0) _flyingState = FlyingState::UP;
		break;

	case FlyingState::UP:
		_panCakeY -= _flyingYSpeed;
		if (_panCakeY < 0) _panCakeY = 0;
		break;

	case FlyingState::MIDDLE_TO_DOWN:
		_flyingTransitionTimer -= 1.0f / 60.0f;
		if (_flyingTransitionTimer <= 0) _flyingState = FlyingState::DOWN;
		break;

	case FlyingState::DOWN:
		_panCakeY += _flyingYSpeed;
		break;
	}

	// 비행 중 타일 충돌 처리 (바닥 아래로 떨어지지 않게)
	for (auto& tile : _tiles)
	{
		RECT playerFeet = RectMake(_playerHitbox.left + 10, _playerHitbox.bottom - 10, (_playerHitbox.right - _playerHitbox.left) - 20, 10);
		RECT intersection;
		if (IntersectRect(&intersection, &playerFeet, &tile))
		{
			// 플레이어를 타일 바로 위로 이동
			_panCakeY -= (_playerHitbox.bottom - tile.top);
			break;
		}
	}

	// 애니메이션 프레임 처리
	_flyingFrameCount++;
	if (_flyingState == FlyingState::UP || _flyingState == FlyingState::DOWN)
	{
		if (_flyingFrameCount % 5 == 0)
		{
			_flyingFrameX = (_flyingFrameX + 1) % 2;
		}
	}
}
