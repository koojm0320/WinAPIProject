#pragma once
#include "Stdafx.h"

// 이미지 출력 방법
// 1. 파일 입출력 2. 리소스에 박고 쓰는 방법
// 외부에서 가져오는 방법이 용이

class GImage
{
public:
	enum IMAGE_LOAD_KIND
	{
		LOAD_RESOURCE = 0, LOAD_FILE,
		LOAD_EMPTY, LOAD_END
	};

	typedef struct tagImage
	{
		DWORD	resID;
		HDC		hMemDC;
		HBITMAP hBit;
		HBITMAP hOBIT;
		int		width;
		int		height;
		BYTE	loadType;

		tagImage()
		{
			resID = 0;
			hMemDC = NULL;
			hBit = NULL;
			hOBIT = NULL;
			width = 0;
			height = 0;
			loadType = LOAD_RESOURCE;
		}
	}IMAGE_INFO, * LPIMAGE_INFO;

private:
	LPIMAGE_INFO	_imageInfo;
	char* _fileName;
	bool			_isTrans;
	// COLORREF: 윈도우에서 색상값을 표현하기 위한 데이터형 32비트 부호없는 정수형 -> RGB: 각각 1바이트 0 ~ 255 범위의 색상
	// 255, 0, 255 -> 마젠타 색상 많이 사용
	COLORREF		_transColor;

public:
	// 빈 비트맵
	HRESULT init(int width, int height);

	// 이미지
	HRESULT init(const char* fileName, int width, int height, bool isTrans = false, COLORREF transColor = RGB(0, 0, 0));

	// 투명 컬러키 세팅
	void setTransColor(bool isTrans, COLORREF transColor);

	// 메모리 해제
	void release(void);

	void render(HDC hdc);
	void render(HDC hdc, int destX, int destY);

	inline HDC getMemDC(void) { return _imageInfo->hMemDC; }

	GImage();
	~GImage() {}
};

