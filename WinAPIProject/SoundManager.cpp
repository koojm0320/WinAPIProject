#include "Stdafx.h"
#include "SoundManager.h"

HRESULT SoundManager::init(void)
{
    // ������� ���
    addSound("BGM", "Resources/Sounds/BackgroundMusic.wav");

    // ȿ���� ���
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

    // ====[ ��ϵ� ��� ���� ������ ä�� ����ŭ �̸� ����Ӵϴ� ]====
    for (auto const& sound : _soundList)
    {
        // BGM�� ä���� �ϳ��� �ʿ��մϴ�.
        if (sound.first == "BGM")
        {
            std::string command = "open \"" + sound.second + "\" type waveaudio alias BGM";
            mciSendString(TEXT(command.c_str()), NULL, 0, NULL);
        }
        // ȿ������ ���� ä���� ����ϴ�.
        else
        {
            for (int i = 0; i < MAX_SOUND_CHANNELS; ++i)
            {
                // �� ä�ο� ������ ���� �ο� (��: "Jelly0", "Jelly1", ...)
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
    // �����ִ� ��� ����� ä���� �ݽ��ϴ�.
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
    // BGM�� �ϳ��� ä�ο��� �ݺ� ����մϴ�.
    std::string command = "play " + key + " repeat";
    mciSendString(TEXT(command.c_str()), NULL, 0, NULL);
}

void SoundManager::stopBGM(std::string key)
{
    std::string command = "stop " + key;
    mciSendString(TEXT(command.c_str()), NULL, 0, NULL);
}

// ====[ ȿ���� ��� �Լ��� ä�� ��ȯ ������� ���� ]====
void SoundManager::playEffect(std::string key)
{
    // ���� ������ ä�� �ε����� �����ɴϴ�.
    int& channel = _channelIndex[key];

    // ä�� ���� ���� (��: "Jelly3")
    std::string alias = key + std::to_string(channel);

    // �ش� ä�ο��� �Ҹ��� ó������ ����մϴ�.
    std::string command = "play " + alias + " from 0";
    mciSendString(TEXT(command.c_str()), NULL, 0, NULL);

    // ���� ����� ���� ä�� �ε����� 1 ������Ű��, �ִ� ä�� ���� ������ 0���� ���ư��ϴ�.
    channel = (channel + 1) % MAX_SOUND_CHANNELS;
}

void SoundManager::addSound(std::string key, std::string fileName)
{
    _soundList.insert(make_pair(key, fileName));
    // ��� ȿ������ ä�� �ε����� 0���� �ʱ�ȭ�մϴ�.
    _channelIndex[key] = 0;
}