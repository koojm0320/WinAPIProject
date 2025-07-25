#pragma once
#include "Stdafx.h"
#include "GameNode.h"

enum class PlayerState
{
	RUNNING,
	SLIDING,
	JUMPING,
	DOUBLE_JUMPING,
	LANDING
};

class MainGame : public GameNode
{
private:
	int _panCakeX;
	float _panCakeY;
	
	int _panCakeFrameX;
	int _panCakeFrameCount;

	float _bgX;
	float _bgObj1X;
	float _bgObj2X;
	float _bgObj3X;

	RECT _playerHitbox;
	std::vector<RECT> _tiles;
	float _mapPosX;

	bool _isDebug;

	PlayerState _playerState;
	//float _groundY;

	float _jumpPower;
	float _gravity;
	float _velocityY;

	bool _canDoubleJump;

	float _landingTime;
	float _landingTimer;


public:
	MainGame();
	~MainGame() {}

	HRESULT init(void) override;
	void release(void) override;
	void update(void) override;
	void render(HDC hdc) override;
};

