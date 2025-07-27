#pragma once
#include "GameNode.h"

enum class HurdleType
{
	LOW,
	HIGH,
	SPIKE
};

class Hurdle
{
private:
	GImage* _image;
	RECT _rc;
	HurdleType _type;
	int _frameX;
	int _frameCount;
	bool _isReady;

	bool _animationStarted;
	bool _animationFinished;
public:
	Hurdle(HurdleType type, int x, int y);
	~Hurdle();

	void update(float mapPosX);
	void render(HDC hdc);

	RECT& getRect() { return _rc; }
};

