#include "Stdafx.h"
#include "ProgressBar.h"

HRESULT ProgressBar::init(int x, int y, int width, int height)
{
	_rc = RectMake(x, y, width, height);
	_progressBarBg = IMAGEMANAGER->findImage("체력바배경");
	_progressBarTop = IMAGEMANAGER->findImage("체력바");
	_width = width; // 초기 너비 설정

	_effectImage = nullptr;
	_effectFrameX = 0;
	_effectFrameCount = 0;

	return S_OK;
}

void ProgressBar::release()
{
}

void ProgressBar::setBackImage(GImage* backImage)
{
	_effectImage = backImage;
}

void ProgressBar::update()
{
	if (_effectImage)
	{
		_effectFrameCount++;
		// 4프레임마다 다음 이미지로 변경 (속도 조절 가능)
		if (_effectFrameCount % 4 == 0)
		{
			_effectFrameX = (_effectFrameX + 1) % (_effectImage->getMaxFrameX() + 1); // 수정된 코드
		}
	}
}

void ProgressBar::render(HDC hdc)
{
	_progressBarBg->alphaRender(hdc, _rc.left, _rc.top, 150);
	_progressBarTop->render(hdc, _rc.left + 3, _rc.top + 1,
		0, 0, (int)_width, _progressBarTop->getHeight());

	if (_effectImage)
	{
		// 이펙트 이미지의 중앙이 체력 바 끝에 오도록 위치를 계산합니다.
		int effectX = _rc.left + _width - (_effectImage->getFrameWidth() / 2);
		// Y좌표를 체력 바의 세로 중앙에 맞춥니다.
		int effectY = _rc.top + (_rc.bottom - _rc.top) / 2 - (_effectImage->getFrameHeight() / 2 + 10);

		_effectImage->frameRender(hdc, effectX, effectY, _effectFrameX, 0);
	}
}

void ProgressBar::setGauge(float currentGauge, float maxGauge)
{
	// 3. 너비 계산 공식을 올바르게 수정합니다.
	_width = (currentGauge / maxGauge) * (_rc.right - _rc.left);
}