#include "Stdafx.h"
#include "Item.h"

Item::Item() : _image(nullptr), _effectImage(nullptr), _isEaten(false), _alpha(255)
{
}

Item::~Item()
{
}

// ���� ������ ���� �̹����� �����ϴ� ������ ���⿡ �����մϴ�.
HRESULT Item::init(ItemType type, int x, int y)
{
	_type = type;

	if (_type == ItemType::JELLY_NORMAL)
	{
		_image = IMAGEMANAGER->findImage("����");
		_effectImage = IMAGEMANAGER->findImage("��������Ʈ");
		_rc = RectMake(x, y, 25, 34);
	}
	else if (_type == ItemType::JELLY_BEAR)
	{
		_image = IMAGEMANAGER->findImage("������");
		_effectImage = IMAGEMANAGER->findImage("����������Ʈ");
		_rc = RectMake(x, y, 55, 51);
	}
	else if (_type == ItemType::ITEM_SPRINT)
	{
		_image = IMAGEMANAGER->findImage("���־�����");
		_effectImage = IMAGEMANAGER->findImage("����������Ʈ");
		_rc = RectMake(x, y, 90, 90);
	}

	return S_OK;
}

void Item::release()
{
}

void Item::update(float mapPosX)
{
	_rc.left += (int)mapPosX;
	_rc.right += (int)mapPosX;

	if (_isEaten && _alpha > 0)
	{
		_alpha -= 10;
		if (_alpha < 0) _alpha = 0;
	}
}

void Item::render(HDC hdc)
{
	if (!_isEaten && _image)
	{
		if (_type == ItemType::ITEM_SPRINT)
		{
			IMAGEMANAGER->frameRender("���־�����", hdc, _rc.left, _rc.top, 0, 0);
		}
		else
		{
			_image->render(hdc, _rc.left, _rc.top);
		}
	}
	else if (_alpha > 0 && _effectImage)
	{
		_effectImage->alphaRenderWithTransparency(hdc, _rc.left, _rc.top, _alpha);
	}
}