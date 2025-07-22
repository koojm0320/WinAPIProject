#include "KeyManager.h"
#include "stdafx.h"

HRESULT KeyManager::init(void)
{
    for (int i = 0; i < KEY_MAX; i++)
    {
        this->setKeyDown(i, false);
        this->setKeyUp(i, false);
    }
    return S_OK;
}

bool KeyManager::isOnceKeyUp(int key) // CPU 우선연산 때문, int가 가장 빠름
{
    if (!(GetAsyncKeyState(key) & 0x8000))
    {
        if (this->getKeyDown()[key])
        {
            this->setKeyDown(key, false);
            return true;
        }
    }
    else this->setKeyDown(key, true);
    return false;
}

bool KeyManager::isOnceKeyDown(int key)
{
    if (GetAsyncKeyState(key) & 0x8000)
    {
        if (!this->getKeyDown()[key])
        {
            this->setKeyDown(key, true);
            return true;
        }
    }
    else this->setKeyDown(key, false);
    return false;
}

bool KeyManager::isStayKeyDown(int key)
{
    if (GetAsyncKeyState(key) & 0x8000)
        return true;
    return false;
}

bool KeyManager::isToggleKey(int key)
{
    if (GetKeyState(key) & 0x0001)
        return true;
    return false;
}
