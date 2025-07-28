#include "Stdafx.h"
#include "ItemManager.h"

HRESULT ItemManager::init()
{
	return S_OK;
}

void ItemManager::release()
{
	for (auto& item : _items)
	{
		SAFE_DELETE(item);
	}
	_items.clear();
}

void ItemManager::update(float mapPosX)
{
	for (size_t i = 0; i < _items.size(); )
	{
		_items[i]->update(mapPosX);

		if (_items[i]->getRect().right < 0)
		{
			SAFE_DELETE(_items[i]);
			_items.erase(_items.begin() + i);
		}
		else
		{
			i++;
		}
	}
}

void ItemManager::render(HDC hdc)
{
	for (auto& item : _items)
	{
		item->render(hdc);
	}
}

void ItemManager::createItems(const std::vector<RECT>& tiles, const std::vector<Hurdle*>& hurdles)
{
	// 타일 위에 일반 젤리 생성
	for (const auto& tile : tiles)
	{
		for (int i = 0; i < 5; ++i)
		{
			Item* newItem = new Item(); // new Item()으로 변경
			newItem->init(ItemType::JELLY_NORMAL, tile.left + i * 25, tile.top - 20);
			_items.push_back(newItem);
		}
	}
	// 장애물 위에 곰젤리 생성
	for (const auto& hurdle : hurdles)
	{
		Item* newItem = new Item(); // new Item()으로 변경
		newItem->init(ItemType::JELLY_BEAR, hurdle->getRect().left + 20, hurdle->getRect().top - 20);
		_items.push_back(newItem);
	}
}

void ItemManager::checkCollision(const RECT& playerRC)
{
	for (auto& item : _items)
	{
		if (!item->isEaten())
		{
			RECT temp;
			if (IntersectRect(&temp, &playerRC, &item->getRect()))
			{
				item->eat();
			}
		}
	}
}