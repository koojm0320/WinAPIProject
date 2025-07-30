#include "Stdafx.h"
#include "MainGame.h"
#include "ItemManager.h"
#include "EffectManager.h"


// ��� ���� �ʱ�ȭ
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
	_jumpPower(20.0f),					// ���� ��
	_gravity(0.97f),					// �߷°�
	_velocityY(0.0f),
	_landingTime(0.1f),					// ���� �ִϸ��̼�
	_landingTimer(0.0f),
	_sprintTimer(0.0f),
	_originalMapSpeed(-8.0f),			// ������Ʈ �⺻ �̵� �ӵ�
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
	_currentHp(100.0f),					// ����HP
	_maxHp(100.0f),						// �ִ�HP
	_damageAlpha(0.0f),
	_isGameOver(false),
	_isShowingDamage(false),
	_isDebug(true),
	//Ablity
	_abilityGaugeTimer(0.0f),
	_abilityChargeTime(15.0f),			// �ɷ� ���� �ð� 
	_isAbilityActive(false),
	_abilityDurationTimer(0.0f),
	_abilityDuration(7.0f),				// �ɷ� ���� �ð�
	_postAbilityInvincibleTimer(0.0f),
	_flyingYSpeed(5.5f),				// ���� �ӵ�
	_flyingState(FlyingState::DOWN),
	_flyingFrameX(0),
	_flyingFrameCount(0),
	_transformationDuration(0.5f),		// ���� �ð�
	_transformationTimer(0.0f),
	_flyingTransitionDuration(0.07f),	// �߰� �ִϸ��̼�
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
	IMAGEMANAGER->addFrameImage("����", "Resources/Images/Object/PanCakeSprint.bmp", 668, 149, 4, 1, true, RGB(255, 0, 255));


	// ��ֹ�
	IMAGEMANAGER->addFrameImage("����غ�", "Resources/Images/Object/hurdleReady.bmp", 138, 131, 2, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("�������", "Resources/Images/Object/hurdle.bmp", 388, 148, 4, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("�������", "Resources/Images/Object/highHurdle.bmp", 475, 296, 5, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("������ũ", "Resources/Images/Object/spike2.bmp", 137, 416, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("��ֹ��ı�", "Resources/Images/Effect/explode.bmp", 1568, 226, 7, 1, true, RGB(255, 0, 255));


	// Ÿ��
	IMAGEMANAGER->addImage("Ÿ��", "Resources/Images/Object/tile.bmp", 129, 50, true, RGB(255, 0, 255));

	// UI
	IMAGEMANAGER->addImage("ü�¾�����", "Resources/Images/UI/iconHeartLife.bmp", 54, 54, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("ü�¹ٹ��", "Resources/Images/UI/gaugebgHeart.bmp", 650, 25, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("ü�¹�", "Resources/Images/UI/gaugeHeartOrange.bmp", 644, 23, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("ü�¹�����Ʈ", "Resources/Images/UI/gaueHeartEffect.bmp", 33, 30, 2, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("������", "Resources/Images/UI/damage.bmp", WINSIZE_X, WINSIZE_Y, true, RGB(255, 0, 255));

	// ����
	IMAGEMANAGER->addImage("����", "Resources/Images/Object/jelly.bmp", 25, 34, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("��������Ʈ", "Resources/Images/Effect/jellyEffect.bmp", 86, 46, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("����������Ʈ", "Resources/Images/Effect/bearJellyEffect.bmp", 96, 56, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("������", "Resources/Images/Object/bearJelly.bmp", 55, 51, true, RGB(255, 0, 255));

	// ������
	IMAGEMANAGER->addFrameImage("���־�����", "Resources/Images/Object/booster.bmp", 240, 60, 4, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("�ڼ�������", "Resources/Images/Object/magnetic.bmp", 240, 60, 4, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("����������Ʈ", "Resources/Images/Effect/itemEaten.bmp", 272, 68, 4, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("��������Ʈ", "Resources/Images/Effect/sprint.bmp", 151, 141, 1, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("�ڼ�����Ʈ", "Resources/Images/Effect/mageticEffect.bmp", 1452, 216, 6, 1, true, RGB(255, 0, 255));

	// ��Ű �ɷ�
	IMAGEMANAGER->addFrameImage("������", "Resources/Images/Object/PanCakeFlyingUp.bmp", 426, 158, 2, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("�����߰�", "Resources/Images/Object/PanCakeFlyingMiddle.bmp", 213, 158, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("�����ϰ�", "Resources/Images/Object/PanCakeFlyingDown.bmp", 426, 158, 2, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("�ɷ°�����Empty", "Resources/Images/UI/ablityEmpty.bmp", 66, 21, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("�ɷ°�����Full", "Resources/Images/UI/ablityFull.bmp", 66, 21, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("���žִϸ��̼�", "Resources/Images/Object/panCakeTransform.bmp", 1750, 155, 10, 1, true, RGB(255, 0, 255));
	
	// ���� �ʱ� ����
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
	_hpBar->setBackImage(IMAGEMANAGER->findImage("ü�¹�����Ʈ"));
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

	
	// ���� ���� ����
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
		_isInvincible = true; // ���� ���� �� �� �̻� �ǰݵ��� �ʵ���
		_invincibleTimer = 9999.0f;
	}
	// ���� ���� �� ����
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
			_playerState = PlayerState::FLYING; // ������ ������ ���� ���·� ��ȯ
			_isAbilityActive = true; // ���� �ɷ� ȿ���� �̶����� ����
		}
	}

	updateAbility();


	// ���� ����Ʈ
	if (_isSprinting && !_isAbilityActive)
	{
		_sprintTimer -= 1.0f / 60.0f;
		// ���� �ð����� ���� ����Ʈ ����
		if (_panCakeFrameCount % 2 == 0)
		{
			_effectManager->createEffect("��������Ʈ", _panCakeX - 30, (int)_panCakeY + 20, false);
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

	// �ڼ� ȿ��
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

	// ���� ���� �� �����ð�
	if (_isPostSprintInvincible)
	{
		_postSprintInvincibleTimer -= 1.0f / 60.0f;
		if (_postSprintInvincibleTimer <= 0)
		{
			_isPostSprintInvincible = false;
		}
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

	// �� �̵� �ӵ�
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
		loadMap(_tiles.back().right);
	}
	

	// Ű �Է� ó��
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

	// ���� ȿ�� ����
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

	// Ÿ�� - �÷��̾� �浹 ó��
	if (_playerState != PlayerState::FLYING)
	{
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
	}

	// �÷��̾� - ������ �⵿ ó��
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

	// ����, �����̵� ó��
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

				if (RND->getInt(2) == 0) // 0 �Ǵ� 1�� �������� ����
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


	// �浹 ó��
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
					// �ı� ��ġ�� ����Ʈ ����
					if (hurdle->getType() == HurdleType::SPIKE)
					{
						// Spike ��ֹ��� ���: �� ���� ��ġ�� ����Ʈ ����
						_effectManager->createEffect("��ֹ��ı�", hurdle->getRect().left - 50, hurdle->getRect().top + 150, true);
					}
					else
					{
						// �� �� ��ֹ��� ���: ���� ��ġ�� ����Ʈ ����
						_effectManager->createEffect("��ֹ��ı�", hurdle->getRect().left - 50, hurdle->getRect().top - 50, true);
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
				_invincibleTimer = 2.5f; // ����
				_panCakeFrameX = 0;
				_panCakeFrameCount = 0;
				_hitAnimationFinished = false;

				_isShowingDamage = true;
				_damageAlpha = 80.0f;
				break;
			}
		}
	}

	// ���� Ÿ�̸�
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

	// �ٴڿ� �߶� �� ���ӿ���
	if (!_isGameOver && _panCakeY > WINSIZE_Y + 300)
	{
		_isShowingDamage = true;
		_damageAlpha = 80.0f;
		_currentHp = 0;
		_isGameOver = true;
	}

	// ������ ����Ʈ
	if (_isShowingDamage)
	{
		_damageAlpha -= 3.0f; // ���������� �ӵ� (�� ���� ����)
		if (_damageAlpha <= 0)
		{
			_damageAlpha = 0;
			_isShowingDamage = false;
		}
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



	IMAGEMANAGER->findImage("���")->loopRender(memDC, &rc, (int)_bgX, 0);
	IMAGEMANAGER->findImage("�����1")->loopRender(memDC, &rc, (int)_bgObj1X, 0);
	IMAGEMANAGER->findImage("�����2")->loopRender(memDC, &rc, (int)_bgObj2X, 0);
	IMAGEMANAGER->findImage("�����3")->loopRender(memDC, &rc, (int)_bgObj3X, 0);

	for (auto& tile : _tiles)
	{
		IMAGEMANAGER->findImage("Ÿ��")->render(memDC, tile.left, tile.top);
	}

	_hurdleManager->render(memDC);
	_itemManager->render(memDC);
	_effectManager->render(memDC);

	// �ڼ� ����Ʈ
	if (_isMagnetActive)
	{
		GImage* magnetEffectImg = IMAGEMANAGER->findImage("�ڼ�����Ʈ");
		if (magnetEffectImg)
		{
			int effectX = _playerHitbox.left + (_playerHitbox.right - _playerHitbox.left - magnetEffectImg->getFrameWidth()) / 2;
			int effectY = _playerHitbox.top + (_playerHitbox.bottom - _playerHitbox.top - magnetEffectImg->getFrameHeight()) / 2;

			magnetEffectImg->alphaFrameRender(memDC, effectX - 10, effectY, _magnetEffectFrameX, 0, 100);
		}
	}

	// �ɷ� ������ render
	IMAGEMANAGER->findImage("�ɷ°�����Empty")->render(memDC, _panCakeX + 60, _panCakeY - 20);
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

	GImage* fullGaugeImg = IMAGEMANAGER->findImage("�ɷ°�����Full");
	if (fullGaugeImg)
	{
		int gaugeWidth = fullGaugeImg->getWidth() * gaugeRatio;
		fullGaugeImg->render(memDC, _panCakeX + 60, _panCakeY - 20, 0, 0, gaugeWidth, fullGaugeImg->getHeight());
	}

	int renderY = (int)_panCakeY;
	bool shouldRenderPlayer = true;

	// �ǰ� ���� ������ �� ������ ó��
	if (_isInvincible && (int)(_invincibleTimer * 10) % 2 == 0)
	{
		shouldRenderPlayer = false;
	}

	// ���� �� ���� ������ �� ������ ó��
	if (_isPostSprintInvincible && (int)(_postSprintInvincibleTimer * 10) % 2 == 0)
	{
		shouldRenderPlayer = false;
	}

	// ���ӿ��� ���¿����� �׻� ���̵���
	if (_playerState == PlayerState::GAMEOVER)
	{
		shouldRenderPlayer = true;
	}

	if (shouldRenderPlayer)
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
		case PlayerState::SPRINTING:
			IMAGEMANAGER->frameRender("����", memDC, _panCakeX, renderY, _panCakeFrameX, 0);
			break;
		case PlayerState::TRANSFORMING:
			IMAGEMANAGER->frameRender("���žִϸ��̼�", memDC, _panCakeX, renderY, _panCakeFrameX, 0);
			break;
		case PlayerState::FLYING:
			switch (_flyingState)
			{
			case FlyingState::UP:
				IMAGEMANAGER->frameRender("������", memDC, _panCakeX, renderY, _flyingFrameX, 0);
				break;
			case FlyingState::DOWN:
				IMAGEMANAGER->frameRender("�����ϰ�", memDC, _panCakeX, renderY, _flyingFrameX, 0);
				break;
			case FlyingState::MIDDLE_TO_UP:
				IMAGEMANAGER->findImage("�����߰�")->render(memDC, _panCakeX, renderY);
				break;
			case FlyingState::MIDDLE_TO_DOWN:
				IMAGEMANAGER->findImage("�����߰�")->render(memDC, _panCakeX, renderY);
				break;
			default:
				break;
			}
		}
	}

	if (_isShowingDamage)
	{
		IMAGEMANAGER->findImage("������")->alphaRenderWithTransparency(memDC, 0, 0, static_cast<BYTE>(_damageAlpha));
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
	std::string jellyData = "-----TTTMSSSTTTTT-L--L--L-TBTTTTTTTTTTTST-L-TST-L-TST-L-TTTTTT-H-T-H-T-H-T-H-T-H-T-H-TTTBTT";
	std::string mapData = "TTT--TTTTSSSTTTTTTLTTLTTLTTTTTT--T--TTTSTTLTTSTTLTTSTTLTTTTTTTTHTTTHTTTHTTTHTTTHTTTHTTTTTTTHHHHHHHHHHHHHHHHHHHHH";

	const int TILE_WIDTH = 129;
	const int TILE_HEIGHT = 50;
	const int GROUND_Y = WINSIZE_Y - 100;

	std::vector<RECT> currentTiles;
	for (size_t i = 0; i < mapData.length(); ++i)
	{
		// T(Ÿ��), L(���� ���), H(���� ���)�� ��� Ÿ���� ����
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
	// �ɷ��� Ȱ�� ������ ��
	else if(_isAbilityActive)
	{
		_abilityDurationTimer -= 1.0f / 60.0f; // ���ӽð� ����
		// ���ӽð��� ������
		if (_abilityDurationTimer <= 0)
		{
			_isAbilityActive = false;
			_abilityGaugeTimer = 0.0f; // ������ Ÿ�̸� �ʱ�ȭ
			_playerState = PlayerState::RUNNING;
			_isPostSprintInvincible = true; // �ɷ� ���� �� ����
			_postSprintInvincibleTimer = 2.0f; // 2�ʰ� ����
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
		_flyingTransitionTimer = _flyingTransitionDuration; // ��ȯ Ÿ�̸� ����
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

	// ���� �� Ÿ�� �浹 ó�� (�ٴ� �Ʒ��� �������� �ʰ�)
	for (auto& tile : _tiles)
	{
		RECT playerFeet = RectMake(_playerHitbox.left + 10, _playerHitbox.bottom - 10, (_playerHitbox.right - _playerHitbox.left) - 20, 10);
		RECT intersection;
		if (IntersectRect(&intersection, &playerFeet, &tile))
		{
			// �÷��̾ Ÿ�� �ٷ� ���� �̵�
			_panCakeY -= (_playerHitbox.bottom - tile.top);
			break;
		}
	}

	// �ִϸ��̼� ������ ó��
	_flyingFrameCount++;
	if (_flyingState == FlyingState::UP || _flyingState == FlyingState::DOWN)
	{
		if (_flyingFrameCount % 5 == 0)
		{
			_flyingFrameX = (_flyingFrameX + 1) % 2;
		}
	}
}
