#include "Stdafx.h"
#include "SoundManager.h"

HRESULT SoundManager::init(void)
{
    // 배경음악
    addSound("BGM", "Resources/Sounds/BackgroundMusic.wav");

    // 효과음
    addSound("Jump1", "Resources/Sounds/Jump1.wav");
    addSound("Jump2", "Resources/Sounds/Jump2.wav");
    addSound("Slide1", "Resources/Sounds/Slide1.wav");
    addSound("Slide2", "Resources/Sounds/Slide2.wav");
    addSound("Jelly", "Resources/Sounds/Jelly.wav");
    addSound("TransformStart", "Resources/Sounds/TransformStart.wav");
    addSound("TransformFlight1", "Resources/Sounds/TransformFlight1.wav");
    addSound("TransformFlight2", "Resources/Sounds/TransformFlight2.wav");
    addSound("TransformFlight3", "Resources/Sounds/TransformFlight3.wav");
    addSound("TransformEnd", "Resources/Sounds/TransformEnd.wav");
    addSound("Crash", "Resources/Sounds/Crash.wav");

    return S_OK;
}

void SoundManager::release(void)
{
    for (auto const& sound : _soundList)
    {
        std::string command = "close " + sound.first;
        mciSendString(TEXT(command.c_str()), NULL, 0, NULL);
    }
    _soundList.clear();
}

void SoundManager::playBGM(std::string key)
{
    auto findResult = _soundList.find(key);
    if (findResult != _soundList.end())
    {
        std::string command = "open \"" + findResult->second + "\" type mpegvideo alias " + key;
        mciSendString(TEXT(command.c_str()), NULL, 0, NULL);

        command = "play " + key + " repeat";
        mciSendString(TEXT(command.c_str()), NULL, 0, NULL);
    }
}

void SoundManager::stopBGM(std::string key)
{
    std::string command = "stop " + key;
    mciSendString(TEXT(command.c_str()), NULL, 0, NULL);
}

void SoundManager::playSound(std::string key)
{
    auto findResult = _soundList.find(key);
    if (findResult != _soundList.end())
    {
        PlaySound(TEXT(findResult->second.c_str()), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
    }
}

void SoundManager::addSound(std::string key, std::string fileName)
{
    _soundList.insert(make_pair(key, fileName));
}