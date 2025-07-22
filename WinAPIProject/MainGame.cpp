#include "Stdafx.h"
#include "MainGame.h"

MainGame::MainGame() : _playerRect{}, _enemySpawnTimer(0), _laserFireTimer(0)
{

}

HRESULT MainGame::init(void)
{
	GameNode::init();

	// 배경 이미지 주소값
	_bgImage = new GImage;
	_bgImage->init("Resources/Images/BackGround/skyBG.bmp", WINSIZE_X, WINSIZE_Y);



	// 플레이어 이미지 주소값
	//_plImage = new GImage;
	//_plImage->init("Resources/Images/Object/Player.bmp", 70, 70, true, RGB(255, 0, 255));

	_playerHP = 10;
	_laserFireTimer = 0;

	RND->init();

	_playerRect = RectMakeCenter(WINSIZE_X / 2, WINSIZE_Y / 2, 100, 100);
	_bgY = 0.0f;

	return S_OK;
}

void MainGame::release(void)
{
	GameNode::release();

	SAFE_DELETE(_bgImage);
	//SAFE_DELETE(_plImage);
}

void MainGame::update(void)
{
	GameNode::update();

	if (KEYMANAGER->isOnceKeyDown('Q'))
	{
		if (MessageBox(_hWnd, "게임을 종료하시겠습니까?", "게임 종료", MB_OKCANCEL))
		{
			PostQuitMessage(0);
		}
	}

	if (KEYMANAGER->isStayKeyDown(VK_UP))
	{
		_playerRect.top -= 10;
		_playerRect.bottom -= 10;
	}
	if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	{
		_playerRect.top += 10;
		_playerRect.bottom += 10;
	}
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		_playerRect.left -= 10;
		_playerRect.right -= 10;
	}
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		_playerRect.left += 10;
		_playerRect.right += 10;
	}

	if (_playerRect.right > WINSIZE_X)
	{
		_playerRect.right = WINSIZE_X;
		_playerRect.left = _playerRect.right - 100;
	}
	if (_playerRect.left < 0)
	{
		_playerRect.left = 0;
		_playerRect.right = _playerRect.left + 100;
	}
	if (_playerRect.bottom > WINSIZE_Y)
	{
		_playerRect.bottom = WINSIZE_Y;
		_playerRect.top = _playerRect.bottom - 100;
	}
	if (_playerRect.top < 0)
	{
		_playerRect.top = 0;
		_playerRect.bottom = _playerRect.top + 100;
	}

	_laserFireTimer++;

	if (_laserFireTimer > 5)
	{
		_laserFireTimer = 0;

		Laser newLaser;

		newLaser.rc = RectMake(_playerRect.left + 45, _playerRect.top - 20, 10, 20);
		newLaser.isFire = true;
		_laser.push_back(newLaser);
	}

	for (size_t i = 0; i < _laser.size(); ++i)
	{
		_laser[i].rc.top -= 20;
		_laser[i].rc.bottom -= 20;

	}

	_enemySpawnTimer++;

	if (_enemySpawnTimer >= 20)
	{
		_enemySpawnTimer = 0; 

		Enemy newEnemy;

		int randomX = RND->getFromIntTo(0, WINSIZE_X - 50);
		newEnemy.rc = RectMake(randomX, 0, 50, 50);
		_enemy.push_back(newEnemy);
	}

	for (size_t i = 0; i < _enemy.size(); ++i)
	{
		_enemy[i].rc.top += 5;
		_enemy[i].rc.bottom += 5;
	}
	
	Collide();

	for (auto iter = _laser.begin(); iter != _laser.end(); )
	{
		if (iter->rc.bottom < 0 || !iter->isFire)
		{
			iter = _laser.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	for (auto iter = _enemy.begin(); iter != _enemy.end(); )
	{
		if (iter->rc.top > WINSIZE_Y || !iter->isAlive)
		{
			iter = _enemy.erase(iter);
		}
		else
		{
			++iter; // 다음으로 이동
		}
	}

	_bgY += 3.0f;

	if (_bgY >= WINSIZE_Y)
	{
		_bgY = 0;
	}
}

void MainGame::render(HDC hdc)
{
	HDC memDC = this->getBackBuffer()->getMemDC();
	PatBlt(memDC, 0, 0, WINSIZE_X, WINSIZE_Y, WHITENESS);
	// ============

	_bgImage->render(memDC, 0, (int)_bgY);
	_bgImage->render(memDC, 0, (int)_bgY - WINSIZE_Y);

	SetTextColor(memDC, RGB(0, 0, 0));
	std::string hpText = "HP: " + std::to_string(_playerHP);
	TextOut(memDC, 20, WINSIZE_Y - 40, hpText.c_str(), hpText.length());

	if (KEYMANAGER->isToggleKey(VK_F1))
	{
		DrawRectMake(memDC, _playerRect);
	}
	
	DrawRectMake(memDC, _playerRect);

	for (size_t i = 0; i < _enemy.size(); ++i)
	{
		DrawRectMake(memDC, _enemy[i].rc);
	}

	for (size_t i = 0; i < _laser.size(); ++i)
	{
		DrawRectMake(memDC, _laser[i].rc);
	}

	//_plImage->render(memDC, _playerRect.left, _playerRect.top);

	// ============
	this->getBackBuffer()->render(hdc, 0, 0);
}

void MainGame::Collide()
{
	RECT tempRect;

	for (size_t i = 0; i < _laser.size(); ++i)
	{
		for (size_t j = 0; j < _enemy.size(); j++)
		{
			if (_laser[i].isFire && _enemy[j].isAlive)
			{
				if (IntersectRect(&tempRect, &_laser[i].rc, &_enemy[j].rc))
				{
					_laser[i].isFire = false;  // 레이저 비활성화
					_enemy[j].isAlive = false; // 적 비활성화
				}
			}
		}
	}

	for (size_t i = 0; i < _enemy.size(); i++)
	{
		if (_enemy[i].isAlive)
		{
			if (IntersectRect(&tempRect, &_enemy[i].rc, &_playerRect))
			{
				_enemy[i].isAlive = false;
				_playerHP--;
				if (_playerHP <= 0)
				{
					MessageBox(_hWnd, "게임 오버 !", "게임 오버", MB_OK);
					PostQuitMessage(0);
				}
			}
		}
	}
}
