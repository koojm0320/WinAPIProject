#pragma once
#include "GameNode.h"

class ProgressBar
{
private:
	RECT _rc; // 프로그레스바 전체 영역
	float _width; // 현재 게이지 너비

	GImage* _progressBarTop;  // 채워지는 이미지 (오렌지색 바)
	GImage* _progressBarBg;   // 배경 이미지 (검은색 바)

	GImage* _effectImage;       // 이펙트 이미지 포인터
	int _effectFrameX;          // 현재 이펙트 프레임
	int _effectFrameCount;      // 프레임 조절용 카운트

public:
	HRESULT init(int x, int y, int width, int height);
	void release();
	void update();
	void render(HDC hdc);

	void setGauge(float currentGauge, float maxGauge);

	// 게이지 뒷부분(오른쪽 끝)에 이펙트를 추가
	void setBackImage(GImage* backImage);
};
