#include "Stdafx.h"
#include "Hurdle.h"

Hurdle::Hurdle(HurdleType type, int x, int y) : _type(type), _frameX(0), _frameCount(0), _isReady(true)
{
	_animationStarted = false;
	_animationFinished = false;

	if (_type == HurdleType::LOW)
	{
		_image = IMAGEMANAGER->findImage("�������");
		_rc = RectMake(x + 20, y, 70, 120);
	}
	else
	{
		_image = IMAGEMANAGER->findImage("�������");
		_rc = RectMake(x + 20, y + 70, 70, 230);
	}
}

Hurdle::~Hurdle()
{
}

void Hurdle::update()
{
    // 1. ����� ȭ�� ������ ���԰�, ���� �ִϸ��̼��� ���۵��� �ʾҴٸ� ����
    if (!_animationStarted && _rc.right < WINSIZE_X)
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
            if (_frameCount % 5 == 0)
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

    // 3. ����� ��ġ�� ��� �������� �̵�
    _rc.left -= 8;
    _rc.right -= 8;
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
			IMAGEMANAGER->frameRender("�������", hdc, _rc.left, _rc.top, _frameX, 0);
		}
		else
		{
			IMAGEMANAGER->frameRender("�������", hdc, _rc.left, _rc.top - 70, _frameX, 0);
		}
	}
}
