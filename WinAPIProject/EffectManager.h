#pragma once
#include "SingletonBase.h"
#include "Effect.h"
#include <vector>

class EffectManager : public SingletonBase<EffectManager>
{
private:
    std::vector<Effect*> _effects;

public:
    HRESULT init();
    void release();
    void update(float mapPosX);
    void render(HDC hdc);

    void createEffect(std::string imageKey, int x, int y, bool isAnimated = true);
};
