#include "Stdafx.h"
#include "Hurdle.h"

Hurdle::Hurdle(HurdleType type, int x, int y) : _type(type), _frameX(0), _frameCount(0), _isReady(true)
{
	_animationStarted = false;
	_animationFinished = false;
    _isDestroyed = false;
    _explodeFrame = 0;
    _explodeFrameCount = 0;

	if (_type == HurdleType::LOW)
	{
		_image = IMAGEMANAGER->findImage("낮은허들");
		_rc = RectMake(x + 20, y + 40, 70, 120);
	}
	else if(_type == HurdleType::HIGH)
	{
		_image = IMAGEMANAGER->findImage("높은허들");
		_rc = RectMake(x + 20, y + 80, 70, 230);
	}
    else if (_type == HurdleType::SPIKE)
    {
        _image = IMAGEMANAGER->findImage("스파이크");
        _rc = RectMake(x, -20, 90, 416);
        _isReady = false;
        _animationFinished = true;
    }

}

Hurdle::~Hurdle()
{
}

void Hurdle::update(float mapPosX)
{
    if (_isDestroyed)
    {
        _explodeFrameCount++;
        if (_explodeFrameCount % 4 == 0)
        {
            _explodeFrame++;
            if (_explodeFrame >= 7)
            {
                _explodeFrame = 6;
            }
            return;
        }
    }

    if (!_animationStarted && _rc.right < WINSIZE_X + 100)
    {
        _animationStarted = true;
    }

    if (_animationStarted && !_animationFinished)
    {
        _frameCount++;


        if (_isReady)
        {
            if (_frameCount % 10 == 0)
            {
                _frameX++;
                if (_frameX >= 2)
                {
                    _isReady = false;
                    _frameX = 0;
                    _frameCount = 0;
                }
            }
        }
        else
        {
            if (_frameCount % 3 == 0)
            {
                _frameX++;

                if (_type == HurdleType::LOW && _frameX >= 3)
                {
                    _frameX = 3; 
                    _animationFinished = true;
                }

                else if (_type == HurdleType::HIGH && _frameX >= 4)
                {
                    _frameX = 4;
                    _animationFinished = true;
                }
            }
        }
    }

    _rc.left += (int)mapPosX;
    _rc.right += (int)mapPosX;
}

void Hurdle::render(HDC hdc)
{
    if (_isDestroyed)
    {
        return;
    }
    else
    {
        if (_isReady)
        {
            IMAGEMANAGER->frameRender("허들준비", hdc, _rc.left + 20, _rc.top + 100, _frameX, 0);
        }
        else
        {
            if (_type == HurdleType::LOW)
            {
                IMAGEMANAGER->frameRender("낮은허들", hdc, _rc.left - 10, _rc.top - 30, _frameX, 0);
            }
            else if (_type == HurdleType::HIGH)
            {
                IMAGEMANAGER->frameRender("높은허들", hdc, _rc.left, _rc.top - 70, _frameX, 0);
            }
            else if (_type == HurdleType::SPIKE)
            {
                _image->render(hdc, _rc.left - 20, _rc.top);
            }
        }
    }
}
