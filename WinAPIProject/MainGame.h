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
	float _abilityGaugeTimer;           // �ɷ� ������ ���� Ÿ�̸�
	float _abilityChargeTime;           // �ɷ� ������ �ʿ��� �ð�
	bool _isAbilityActive;              // �ɷ� Ȱ��ȭ ����
	float _abilityDurationTimer;        // �ɷ� ���ӽð� Ÿ�̸�
	float _abilityDuration;             // �� �ɷ� ���ӽð�
	float _postAbilityInvincibleTimer;  // �ɷ� ���� �� ���� Ÿ�̸�
	float _flyingYSpeed;                // ���� �� Y�� �̵� �ӵ�
	FlyingState _flyingState;           // ���� ���� ���� (���, �ϰ� ��)
	int _flyingFrameX;					// ���� �ִϸ��̼� ������
	int _flyingFrameCount;				// ���� �ִϸ��̼� ������ ī��Ʈ

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

