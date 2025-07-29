#pragma once
#include "GameNode.h"

class Effect
{
private:
    GImage* _image;
    RECT _rc;
    int _frameX;
    int _frameCount;
    bool _isFinished;
    bool _isAnimated;

public:
    Effect(GImage* image, int x, int y, bool isAnimated = true);
    ~Effect();

    void update(float mapPosX);
    void render(HDC hdc);

    bool isFinished() { return _isFinished; }
    RECT getRect() { return _rc; }
};
