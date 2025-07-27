#include "Stdafx.h"
#include "Hurdle.h"

Hurdle::Hurdle(HurdleType type, int x, int y) : _type(type), _frameX(0), _frameCount(0), _isReady(true)
{
	_animationStarted = false;
	_animationFinished = false;

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
    // 1. 허들이 화면 안으로 들어왔고, 아직 애니메이션이 시작되지 않았다면 시작
    if (!_animationStarted && _rc.right < WINSIZE_X + 100)
    {
        _animationStarted = true;
    }

    // 2. 애니메이션이 시작되었고, 아직 끝나지 않았다면 프레임 업데이트
    if (_animationStarted && !_animationFinished)
    {
        _frameCount++;

        // 허들 준비 애니메이션 (2프레임)
        if (_isReady)
        {
            if (_frameCount % 10 == 0)
            {
                _frameX++;
                if (_frameX >= 2) // 준비 애니메이션이 끝나면
                {
                    _isReady = false; // 허들 등장 상태로 전환
                    _frameX = 0;
                    _frameCount = 0;
                }
            }
        }
        // 허들 등장 애니메이션
        else
        {
            if (_frameCount % 3 == 0)
            {
                _frameX++;
                // 낮은 허들의 마지막 프레임에 도달하면
                if (_type == HurdleType::LOW && _frameX >= 3)
                {
                    _frameX = 3; // 마지막 프레임(3)으로 고정
                    _animationFinished = true; // 애니메이션 종료
                }
                // 높은 허들의 마지막 프레임에 도달하면
                else if (_type == HurdleType::HIGH && _frameX >= 4)
                {
                    _frameX = 4; // 마지막 프레임(4)으로 고정
                    _animationFinished = true; // 애니메이션 종료
                }
            }
        }
    }

    _rc.left += (int)mapPosX; // 맵의 x축 이동량만큼 함께 이동
    _rc.right += (int)mapPosX;
}

void Hurdle::render(HDC hdc)
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
