#pragma once
#include "SingletonBase.h"
#include "Item.h"
#include "Hurdle.h"

class ItemManager : public SingletonBase<ItemManager>
{
private:
	std::vector<Item*> _items;

public:
	HRESULT init();
	void release();
	void update(float mapPosX);
	void render(HDC hdc);

	void createItems(const std::string& jellyData, float startX);
	void checkCollision(const RECT& playerRC);

};

