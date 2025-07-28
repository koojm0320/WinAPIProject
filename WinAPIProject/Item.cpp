#include "Stdafx.h"
#include "Item.h"

Item::Item() : _image(nullptr), _effectImage(nullptr), _isEaten(false), _alpha(255)
{
}

Item::~Item()
{
}

// Á©¸® Á¾·ù¿¡ µû¶ó ÀÌ¹ÌÁö¸¦ ¼³Á¤ÇÏ´Â ·ÎÁ÷À» ¿©±â¿¡ ±¸ÇöÇÕ´Ï´Ù.
HRESULT Item::init(ItemType type, int x, int y)
{
	_type = type;

	if (_type == ItemType::JELLY_NORMAL)
	{
		_image = IMAGEMANAGER->findImage("Á©¸®");
		_effectImage = IMAGEMANAGER->findImage("Á©¸®ÀÌÆåÆ®");
		_rc = RectMake(x, y, 25, 34);
	}
	else if (_type == ItemType::JELLY_BEAR)
	{
		_image = IMAGEMANAGER->findImage("°õÁ©¸®");
		_effectImage = IMAGEMANAGER->findImage("°õÁ©¸®ÀÌÆåÆ®");
		_rc = RectMake(x, y, 55, 51);
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
		_alpha -= 15;
		if (_alpha < 0) _alpha = 0;
	}
}

void Item::render(HDC hdc)
{
	if (!_isEaten && _image)
	{
		_image->render(hdc, _rc.left, _rc.top);
	}
	else if (_alpha > 0 && _effectImage)
	{
		_effectImage->alphaRender(hdc, _rc.left, _rc.top, _alpha);
	}
}