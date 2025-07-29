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
	GAMEOVER
};

class MainGame : public GameNode
{
private:
	// Player
	int _panCakeX;
	float _panCakeY;
	int _panCakeFrameX;
	int _panCakeFrameCount;

	RECT _playerHitbox;
	PlayerState _playerState;

	float _jumpPower;
	float _gravity;
	float _velocityY;
	float _landingTime;
	float _landingTimer;
	
	bool _canDoubleJump;
	bool _hitAnimationFinished;
	bool _gameOverAnimationFinished;

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
	bool _isInvincible;
	float _invincibleTimer;
	
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

private:
	//void loadMap();
	//void resetMap();
	void loadMap(float startX);

};

