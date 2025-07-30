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
	TRANSFORMING,
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
	int _magnetEffectFrameX;
	int _magnetEffectFrameCount;

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
	bool _isMagnetActive;               

	// Player Physics
	float _jumpPower;
	float _gravity;
	float _velocityY;
	float _landingTime;
	float _landingTimer;
	float _sprintTimer;
	float _magnetTimer;
	float _originalMapSpeed;
	float _invincibleTimer;
	float _postSprintInvincibleTimer;

	// Player Ablity
	float _abilityGaugeTimer;           
	float _abilityChargeTime;           
	bool _isAbilityActive;              
	float _abilityDurationTimer;        
	float _abilityDuration;             
	float _postAbilityInvincibleTimer;  
	float _flyingYSpeed;                
	FlyingState _flyingState;           
	int _flyingFrameX;					
	int _flyingFrameCount;
	float _transformationTimer;		// 변신 애니메이션 타이머
	float _transformationDuration;	// 변신 애니메이션 지속시간
	float _flyingTransitionTimer;       // 비행 전환 애니메이션 타이머
	float _flyingTransitionDuration;    // 비행 전환 애니메이션 지속시간

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

