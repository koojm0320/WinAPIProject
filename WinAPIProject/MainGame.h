#pragma once
#include "Stdafx.h"
#include "GameNode.h"

class MainGame : public GameNode
{
private:
	GImage* _bgImage;
	GImage* _plImage;

	float _bgX;

public:
	MainGame() {};
	~MainGame() {};

	HRESULT init(void) override;
	void release(void) override;
	void update(void) override;
	void render(HDC hdc) override;
};

