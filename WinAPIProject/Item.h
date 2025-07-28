#pragma once
#include "GameNode.h"

enum class ItemType
{
	JELLY_NORMAL,
	JELLY_BEAR,
	ITEM_DASH,
	ITEM_MAGNET
};

class Item
{
private:
	RECT _rc;
	GImage* _image;
	GImage* _effectImage;
	ItemType _type;

	bool _isEaten;
	int _alpha;

public:
	Item();
	~Item();

	HRESULT init(ItemType type, int x, int y);
	void release();
	void update(float mapPosX);
	void render(HDC hdc);

	void eat() { _isEaten = true; }
	bool isEaten() { return _isEaten; }
	RECT getRect() { return _rc; }
};

