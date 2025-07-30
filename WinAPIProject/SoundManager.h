#pragma once
#include "SingletonBase.h"
#include <map>

class SoundManager : public SingletonBase<SoundManager>
{
private:
    std::map<std::string, std::string> _soundList;

public:
    HRESULT init(void);
    void release(void);

    void playBGM(std::string key);
    void stopBGM(std::string key);

    void playSound(std::string key);

private:
    void addSound(std::string key, std::string fileName);
};

