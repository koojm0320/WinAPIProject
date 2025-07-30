#include "Stdafx.h"
#include "SoundManager.h"

HRESULT SoundManager::init(void)
{
    // 배경음악 등록
    addSound("BGM", "Resources/Sounds/BackgroundMusic.wav");

    // 효과음 등록
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

    // ====[ 등록된 모든 사운드 파일을 채널 수만큼 미리 열어둡니다 ]====
    for (auto const& sound : _soundList)
    {
        // BGM은 채널이 하나만 필요합니다.
        if (sound.first == "BGM")
        {
            std::string command = "open \"" + sound.second + "\" type waveaudio alias BGM";
            mciSendString(TEXT(command.c_str()), NULL, 0, NULL);
        }
        // 효과음은 여러 채널을 만듭니다.
        else
        {
            for (int i = 0; i < MAX_SOUND_CHANNELS; ++i)
            {
                // 각 채널에 고유한 별명 부여 (예: "Jelly0", "Jelly1", ...)
                std::string alias = sound.first + std::to_string(i);
                std::string command = "open \"" + sound.second + "\" type waveaudio alias " + alias;
                mciSendString(TEXT(command.c_str()), NULL, 0, NULL);
            }
        }
    }

    return S_OK;
}

void SoundManager::release(void)
{
    // 열려있는 모든 사운드와 채널을 닫습니다.
    for (auto const& sound : _soundList)
    {
        if (sound.first == "BGM")
        {
            std::string command = "close BGM";
            mciSendString(TEXT(command.c_str()), NULL, 0, NULL);
        }
        else
        {
            for (int i = 0; i < MAX_SOUND_CHANNELS; ++i)
            {
                std::string alias = sound.first + std::to_string(i);
                std::string command = "close " + alias;
                mciSendString(TEXT(command.c_str()), NULL, 0, NULL);
            }
        }
    }
    _soundList.clear();
}

void SoundManager::playBGM(std::string key)
{
    // BGM은 하나의 채널에서 반복 재생합니다.
    std::string command = "play " + key + " repeat";
    mciSendString(TEXT(command.c_str()), NULL, 0, NULL);
}

void SoundManager::stopBGM(std::string key)
{
    std::string command = "stop " + key;
    mciSendString(TEXT(command.c_str()), NULL, 0, NULL);
}

// ====[ 효과음 재생 함수를 채널 순환 방식으로 수정 ]====
void SoundManager::playEffect(std::string key)
{
    // 현재 사운드의 채널 인덱스를 가져옵니다.
    int& channel = _channelIndex[key];

    // 채널 별명 생성 (예: "Jelly3")
    std::string alias = key + std::to_string(channel);

    // 해당 채널에서 소리를 처음부터 재생합니다.
    std::string command = "play " + alias + " from 0";
    mciSendString(TEXT(command.c_str()), NULL, 0, NULL);

    // 다음 재생을 위해 채널 인덱스를 1 증가시키고, 최대 채널 수를 넘으면 0으로 돌아갑니다.
    channel = (channel + 1) % MAX_SOUND_CHANNELS;
}

void SoundManager::addSound(std::string key, std::string fileName)
{
    _soundList.insert(make_pair(key, fileName));
    // 모든 효과음의 채널 인덱스를 0으로 초기화합니다.
    _channelIndex[key] = 0;
}