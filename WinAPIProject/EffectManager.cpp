#include "Stdafx.h"
#include "EffectManager.h"

HRESULT EffectManager::init()
{
    return S_OK;
}

void EffectManager::release()
{
    for (auto& effect : _effects)
    {
        SAFE_DELETE(effect);
    }
    _effects.clear();
}

void EffectManager::update()
{
    for (size_t i = 0; i < _effects.size(); )
    {
        _effects[i]->update();
        if (_effects[i]->isFinished())
        {
            SAFE_DELETE(_effects[i]);
            _effects.erase(_effects.begin() + i);
        }
        else
        {
            i++;
        }
    }
}

void EffectManager::render(HDC hdc)
{
    for (auto& effect : _effects)
    {
        effect->render(hdc);
    }
}

void EffectManager::createEffect(std::string imageKey, int x, int y)
{
    GImage* image = IMAGEMANAGER->findImage(imageKey);
    if (image)
    {
        _effects.push_back(new Effect(image, x, y));
    }
}