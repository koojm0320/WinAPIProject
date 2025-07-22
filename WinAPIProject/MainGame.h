#pragma once
#include "Stdafx.h"
#include "GameNode.h"

struct Enemy
{
	RECT rc;
	bool isAlive = true;
};

struct Laser
{
	RECT rc;
	bool isFire = false;
};

class MainGame : public GameNode
{
private:
	GImage* _bgImage;
	GImage* _plImage;

	int _playerHP;

	RECT _playerRect;
	std::vector<Enemy> _enemy;
	std::vector<Laser> _laser;

	int _enemySpawnTimer;
	int _laserFireTimer;

	float _bgY;

public:
	MainGame();

	HRESULT init(void) override;
	void release(void) override;
	void update(void) override;
	void render(HDC hdc) override;

	void Collide();
};

