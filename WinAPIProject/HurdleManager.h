#pragma once
#include "SingletonBase.h"
#include "Hurdle.h"

class HurdleManager : public SingletonBase<HurdleManager>
{
private:
	std::vector<Hurdle*> _hurdles;

public:
	HRESULT init();
	void release();
	void update(float mapPosX);
	void render(HDC hdc);

	void createHurdle(HurdleType type, int tileX, int tileY);

	std::vector<Hurdle*>& getHurdles() { return _hurdles; }
};