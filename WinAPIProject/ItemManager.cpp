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

		if (_items[i]->getRect().right < 0 || _items[i]->isFinished())
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



void ItemManager::createItems(const std::string& jellyData, float startX)
{
    const int TILE_WIDTH = 129;
    const int GROUND_Y = WINSIZE_Y - 100;

    for (size_t i = 0; i < jellyData.length(); ++i)
    {
        int currentX = startX + (i * TILE_WIDTH);
        char jellyType = jellyData[i];

        switch (jellyType)
        {
        case 'T': // ÀÏ¹Ý Á÷¼± Á©¸®
        {
            for (int j = 0; j < 3; ++j)
            {
                Item* newItem = new Item();
                newItem->init(ItemType::JELLY_NORMAL, currentX + j * 42, GROUND_Y - 70);
                _items.push_back(newItem);
            }
            break;
        }
        case 'L': // ³·Àº Æ÷¹°¼± Á©¸®
        {
            int jellyCount = 8;
            float arcHeight = 120.0f;
            int arcWidth = TILE_WIDTH * 2;

            for (int j = 0; j < jellyCount; ++j)
            {
                float progress = static_cast<float>(j) / (jellyCount - 1);
                float parabolaX = currentX + progress * arcWidth;
                float parabolaY = -4 * arcHeight * progress * (progress - 1.0f);

                Item* newItem = new Item();
                newItem->init(ItemType::JELLY_NORMAL, parabolaX - 90, GROUND_Y - 120 - parabolaY);
                _items.push_back(newItem);
            }
            break;
        }
        case 'H': // ³ôÀº Æ÷¹°¼± Á©¸®
        {
            int jellyCount = 12;
            float arcHeight = 220.0f;
            int arcWidth = TILE_WIDTH * 2; // ³ôÀº Æ÷¹°¼±Àº ´õ ³Ð°Ô

            for (int j = 0; j < jellyCount; ++j)
            {
                float progress = static_cast<float>(j) / (jellyCount - 1);
                float parabolaX = currentX + progress * arcWidth;
                float parabolaY = -4 * arcHeight * progress * (progress - 1.0f);

                Item* newItem = new Item();
                newItem->init(ItemType::JELLY_NORMAL, parabolaX - 90, GROUND_Y - 250 - parabolaY);
                _items.push_back(newItem);
            }
            break;
        }
        case 'S': // °õÁ©¸®
        {
            Item* newItem = new Item();
            newItem->init(ItemType::JELLY_BEAR, currentX + (TILE_WIDTH / 2) - 45, GROUND_Y - 80);
            _items.push_back(newItem);
            break;
        }
        case 'B': // ÁúÁÖ ¾ÆÀÌÅÛ
        {
            Item* newItem = new Item();
            newItem->init(ItemType::ITEM_SPRINT, currentX + 30, GROUND_Y -90); // yÁÂÇ¥´Â ÀûÀýÈ÷ Á¶Àý
            _items.push_back(newItem);
            break;
        }
        case '-': // °ø¹é
        default:
            // ¾Æ¹«°Íµµ »ý¼ºÇÏÁö ¾ÊÀ½
            break;
        }
    }
}


void ItemManager::checkCollision(const RECT& playerRC)
{
    for (auto& item : _items)
    {
        if (!item->isEaten() && (item->getType() == ItemType::JELLY_NORMAL || item->getType() == ItemType::JELLY_BEAR))
        {
            RECT jellyHitbox = playerRC;

            int shiftAmount = 80;
            jellyHitbox.left -= shiftAmount;
            jellyHitbox.right -= shiftAmount;


            RECT temp;
            if (IntersectRect(&temp, &jellyHitbox, &item->getRect()))
            {
                item->eat();
                // °õÁ©¸®¸¦ ¸Ô¾úÀ» ¶§ ÀÌÆåÆ® »ý¼º
                if (item->getType() == ItemType::JELLY_BEAR)
                {
                    EFFECTMANAGER->createEffect("°õÁ©¸®ÀÌÆåÆ®", item->getRect().left, item->getRect().top);
                }
                // ÀÏ¹Ý Á©¸®¸¦ ¸Ô¾úÀ» ¶§ ÀÌÆåÆ® »ý¼º
                else if (item->getType() == ItemType::JELLY_NORMAL)
                {
                    EFFECTMANAGER->createEffect("Á©¸®ÀÌÆåÆ®", item->getRect().left, item->getRect().top);
                }
            }
        }
    }
}