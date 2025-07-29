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

	bool _isDestroyed;
	int _explodeFrame;
	int _explodeFrameCount;
public:
	Hurdle(HurdleType type, int x, int y);
	~Hurdle();

	void update(float mapPosX);
	void render(HDC hdc);

	RECT& getRect() { return _rc; }

	HurdleType getType() const { return _type; }

	void destroy() { _isDestroyed = true; }
	bool isDestroyed() { return _isDestroyed; }
	bool isExplosionFinished() { return _isDestroyed && _explodeFrame >= 6; }
};

