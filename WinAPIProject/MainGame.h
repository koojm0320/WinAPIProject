#pragma once
#include "Stdafx.h"
#include "GameNode.h"

class MainGame : public GameNode
{
private:
	GImage* _bgImage;
	GImage* _plImage;

	int _playerFrameCount;
	int _playerFrameX;

	int _panCakeX;
	int _panCakeY;
	int _panCakeFrameX;
	int _panCakeFrameCount;

	float _bgX;

public:
	MainGame() {};
	~MainGame() {};

	HRESULT init(void) override;
	void release(void) override;
	void update(void) override;
	void render(HDC hdc) override;
};

