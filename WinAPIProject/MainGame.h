#pragma once
#include "Stdafx.h"
#include "GameNode.h"
#include "ItemManager.h"

enum class PlayerState
{
	RUNNING,
	SLIDING,
	JUMPING,
	DOUBLE_JUMPING,
	LANDING,
	HIT,
	SPRINTING,
	FLYING,
	GAMEOVER
};

enum class FlyingState
{
	UP,
	DOWN,
	MIDDLE_TO_UP,
	MIDDLE_TO_DOWN
};

class MainGame : public GameNode
{
private:
#pragma region Player
	
	// PlayerLocation
	int _panCakeX;
	float _panCakeY;
	int _panCakeFrameX;
	int _panCakeFrameCount;

	// PlayerHitBox
	RECT _playerHitbox;
	
	// PlayerState
	PlayerState _playerState;
	bool _canDoubleJump;
	bool _hitAnimationFinished;
	bool _gameOverAnimationFinished;
	bool _isSprinting;
	bool _isInvincible;
	bool _isPostSprintInvincible;

	// Player Physics
	float _jumpPower;
	float _gravity;
	float _velocityY;
	float _landingTime;
	float _landingTimer;
	float _sprintTimer;
	float _originalMapSpeed;
	float _invincibleTimer;
	float _postSprintInvincibleTimer;

	// Player Ablity
	float _abilityGaugeTimer;           // 능력 게이지 충전 타이머
	float _abilityChargeTime;           // 능력 충전에 필요한 시간
	bool _isAbilityActive;              // 능력 활성화 여부
	float _abilityDurationTimer;        // 능력 지속시간 타이머
	float _abilityDuration;             // 총 능력 지속시간
	float _postAbilityInvincibleTimer;  // 능력 종료 후 무적 타이머
	float _flyingYSpeed;                // 비행 시 Y축 이동 속도
	FlyingState _flyingState;           // 비행 세부 상태 (상승, 하강 등)
	int _flyingFrameX;					// 비행 애니메이션 프레임
	int _flyingFrameCount;				// 비행 애니메이션 프레임 카운트

#pragma endregion


	// BG
	float _bgX;
	float _bgObj1X;
	float _bgObj2X;
	float _bgObj3X;
	float _mapPosX;

	//Object

	std::vector<RECT> _tiles;

	ItemManager* _itemManager;
	HurdleManager* _hurdleManager;
	EffectManager* _effectManager;
	
	// UI & etc.
	ProgressBar* _hpBar;
	float _currentHp;
	float _maxHp;
	float _damageAlpha;
	bool _isGameOver;
	bool _isShowingDamage;
	bool _isDebug;

public:
	MainGame();
	~MainGame() {}

	HRESULT init(void) override;
	void release(void) override;
	void update(void) override;
	void render(HDC hdc) override;

	void updateAbility();
	void updateFlying();

private:
	//void loadMap();
	//void resetMap();
	void loadMap(float startX);

};

