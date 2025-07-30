#pragma once
#include "SingletonBase.h"
#include <map>
#include <string>
#include <vector>

// 동시에 재생할 수 있는 효과음의 최대 개수 (채널 수)
const int MAX_SOUND_CHANNELS = 10;

class SoundManager : public SingletonBase<SoundManager>
{
private:
    // 사운드 파일 경로를 저장하는 map
    std::map<std::string, std::string> _soundList;
    // 각 사운드별 현재 채널 인덱스를 저장하는 map
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