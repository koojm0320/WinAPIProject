#include "Stdafx.h"
#include "Effect.h"

Effect::Effect(GImage* image, int x, int y, bool isAnimated)
    : _image(image), _frameX(0), _frameCount(0), _isFinished(false), _isAnimated(isAnimated)
{
    _rc = RectMake(x, y, _image->getFrameWidth(), _image->getFrameHeight());
}

Effect::~Effect() {}

void Effect::update(float mapPosX)
{
    _rc.left += (int)mapPosX;
    _rc.right += (int)mapPosX;

    if (_isAnimated)
    {
        _frameCount++;
        if (_frameCount % 4 == 0)
        {
            _frameX++;
            if (_frameX > _image->getMaxFrameX())
            {
                _isFinished = true;
            }
        }
    }
}

void Effect::render(HDC hdc)
{
    if (!_isFinished)
    {
        int currentFrame = _isAnimated ? _frameX : 0;
        _image->frameRender(hdc, _rc.left, _rc.top, currentFrame, 0);
    }
}