#include "Stdafx.h"
#include "HurdleManager.h"

HRESULT HurdleManager::init()
{
	return S_OK;
}

void HurdleManager::release()
{
	for (auto& hurdle : _hurdles)
	{
		SAFE_DELETE(hurdle);
	}
	_hurdles.clear();
}

void HurdleManager::update(float mapPosX)
{
	for (size_t i = 0; i < _hurdles.size();)
	{
		_hurdles[i]->update(mapPosX);
		if (_hurdles[i]->getRect().right < 0)
		{
			SAFE_DELETE(_hurdles[i]);
			_hurdles.erase(_hurdles.begin() + i);
		}
		else
		{
			i++;
		}
	}
}

void HurdleManager::render(HDC hdc)
{
	for (auto& hurdle : _hurdles)
	{
		hurdle->render(hdc);
	}
}

void HurdleManager::createHurdle(HurdleType type, int tileX, int tileY)
{
	int yPos = (type == HurdleType::LOW) ? tileY - 148 : tileY - 296;
	_hurdles.push_back(new Hurdle(type, tileX, yPos));
}