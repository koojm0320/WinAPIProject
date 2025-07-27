#include "Stdafx.h"
#include "ProgressBar.h"

HRESULT ProgressBar::init(int x, int y, int width, int height)
{
	_rc = RectMake(x, y, width, height);
	_progressBarBg = IMAGEMANAGER->findImage("ü�¹ٹ��");
	_progressBarTop = IMAGEMANAGER->findImage("ü�¹�");
	_width = width; // �ʱ� �ʺ� ����

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
		// 4�����Ӹ��� ���� �̹����� ���� (�ӵ� ���� ����)
		if (_effectFrameCount % 4 == 0)
		{
			_effectFrameX = (_effectFrameX + 1) % (_effectImage->getMaxFrameX() + 1); // ������ �ڵ�
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
		// ����Ʈ �̹����� �߾��� ü�� �� ���� ������ ��ġ�� ����մϴ�.
		int effectX = _rc.left + _width - (_effectImage->getFrameWidth() / 2);
		// Y��ǥ�� ü�� ���� ���� �߾ӿ� ����ϴ�.
		int effectY = _rc.top + (_rc.bottom - _rc.top) / 2 - (_effectImage->getFrameHeight() / 2 + 10);

		_effectImage->frameRender(hdc, effectX, effectY, _effectFrameX, 0);
	}
}

void ProgressBar::setGauge(float currentGauge, float maxGauge)
{
	// 3. �ʺ� ��� ������ �ùٸ��� �����մϴ�.
	_width = (currentGauge / maxGauge) * (_rc.right - _rc.left);
}