#pragma once
#include "SingletonBase.h"
#include <map>
#include <string>
#include <vector>

// ���ÿ� ����� �� �ִ� ȿ������ �ִ� ���� (ä�� ��)
const int MAX_SOUND_CHANNELS = 10;

class SoundManager : public SingletonBase<SoundManager>
{
private:
    // ���� ���� ��θ� �����ϴ� map
    std::map<std::string, std::string> _soundList;
    // �� ���庰 ���� ä�� �ε����� �����ϴ� map
    std::map<std::string, int> _channelIndex;

public:
    HRESULT init(void);
    void release(void);

    void playBGM(std::string key);
    void stopBGM(std::string key);

    void playEffect(std::string key);

private:
    void addSound(std::string key, std::string fileName);
};