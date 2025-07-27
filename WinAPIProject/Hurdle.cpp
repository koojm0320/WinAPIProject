#include "Stdafx.h"
#include "Hurdle.h"

Hurdle::Hurdle(HurdleType type, int x, int y) : _type(type), _frameX(0), _frameCount(0), _isReady(true)
{
	_animationStarted = false;
	_animationFinished = false;

	if (_type == HurdleType::LOW)
	{
		_image = IMAGEMANAGER->findImage("�������");
		_rc = RectMake(x + 20, y + 40, 70, 120);
	}
	else if(_type == HurdleType::HIGH)
	{
		_image = IMAGEMANAGER->findImage("�������");
		_rc = RectMake(x + 20, y + 80, 70, 230);
	}
    else if (_type == HurdleType::SPIKE)
    {
        _image = IMAGEMANAGER->findImage("������ũ");
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
    // 1. ����� ȭ�� ������ ���԰�, ���� �ִϸ��̼��� ���۵��� �ʾҴٸ� ����
    if (!_animationStarted && _rc.right < WINSIZE_X + 100)
    {
        _animationStarted = true;
    }

    // 2. �ִϸ��̼��� ���۵Ǿ���, ���� ������ �ʾҴٸ� ������ ������Ʈ
    if (_animationStarted && !_animationFinished)
    {
        _frameCount++;

        // ��� �غ� �ִϸ��̼� (2������)
        if (_isReady)
        {
            if (_frameCount % 10 == 0)
            {
                _frameX++;
                if (_frameX >= 2) // �غ� �ִϸ��̼��� ������
                {
                    _isReady = false; // ��� ���� ���·� ��ȯ
                    _frameX = 0;
                    _frameCount = 0;
                }
            }
        }
        // ��� ���� �ִϸ��̼�
        else
        {
            if (_frameCount % 3 == 0)
            {
                _frameX++;
                // ���� ����� ������ �����ӿ� �����ϸ�
                if (_type == HurdleType::LOW && _frameX >= 3)
                {
                    _frameX = 3; // ������ ������(3)���� ����
                    _animationFinished = true; // �ִϸ��̼� ����
                }
                // ���� ����� ������ �����ӿ� �����ϸ�
                else if (_type == HurdleType::HIGH && _frameX >= 4)
                {
                    _frameX = 4; // ������ ������(4)���� ����
                    _animationFinished = true; // �ִϸ��̼� ����
                }
            }
        }
    }

    _rc.left += (int)mapPosX; // ���� x�� �̵�����ŭ �Բ� �̵�
    _rc.right += (int)mapPosX;
}

void Hurdle::render(HDC hdc)
{
	if (_isReady)
	{
		IMAGEMANAGER->frameRender("����غ�", hdc, _rc.left + 20, _rc.top + 100, _frameX, 0);
	}
	else
	{
		if (_type == HurdleType::LOW)
		{
			IMAGEMANAGER->frameRender("�������", hdc, _rc.left - 10, _rc.top - 30, _frameX, 0);
		}
		else if (_type == HurdleType::HIGH)
		{
			IMAGEMANAGER->frameRender("�������", hdc, _rc.left, _rc.top - 70, _frameX, 0);
		}
        else if (_type == HurdleType::SPIKE)
        {
            _image->render(hdc, _rc.left - 20, _rc.top);
        }
	}
}
