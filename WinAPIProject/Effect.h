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

public:
    Effect(GImage* image, int x, int y);
    ~Effect();

    void update();
    void render(HDC hdc);

    bool isFinished() { return _isFinished; }
};
