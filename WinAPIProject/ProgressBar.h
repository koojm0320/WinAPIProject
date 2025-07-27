#pragma once
#include "GameNode.h"

class ProgressBar
{
private:
	RECT _rc; // ���α׷����� ��ü ����
	float _width; // ���� ������ �ʺ�

	GImage* _progressBarTop;  // ä������ �̹��� (�������� ��)
	GImage* _progressBarBg;   // ��� �̹��� (������ ��)

	GImage* _effectImage;       // ����Ʈ �̹��� ������
	int _effectFrameX;          // ���� ����Ʈ ������
	int _effectFrameCount;      // ������ ������ ī��Ʈ

public:
	HRESULT init(int x, int y, int width, int height);
	void release();
	void update();
	void render(HDC hdc);

	void setGauge(float currentGauge, float maxGauge);

	// ������ �޺κ�(������ ��)�� ����Ʈ�� �߰�
	void setBackImage(GImage* backImage);
};
