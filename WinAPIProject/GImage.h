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

		int		maxFrameX;     
		int		maxFrameY;     
		int		currentFrameX; 
		int		currentFrameY; 
		int		frameWidth;    
		int		frameHeight;

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

			maxFrameX = 0;
			maxFrameY = 0;
			currentFrameX = 0;
			currentFrameY = 0;
			frameWidth = 0;
			frameHeight = 0;
		}
	}IMAGE_INFO, * LPIMAGE_INFO;

private:
	LPIMAGE_INFO	_imageInfo;
	char* _fileName;
	bool			_isTrans;
	COLORREF		_transColor;

public:
	HRESULT init(int width, int height);
	HRESULT init(const char* fileName, int width, int height, bool isTrans = false, COLORREF transColor = RGB(0, 0, 0));
	HRESULT init(const char* fileName, int width, int height, int maxFrameX, int maxFrameY, bool isTrans = false, COLORREF transColor = RGB(0, 0, 0));


	void setTransColor(bool isTrans, COLORREF transColor);
	void release(void);

	void render(HDC hdc);
	void render(HDC hdc, int destX, int destY);
	void render(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight);

	void frameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY);
	void loopRender(HDC hdc, const LPRECT drawArea, int offSetX, int offSetY);

	void alphaRender(HDC hdc, int destX, int destY, BYTE alpha);

	inline HDC getMemDC(void) { return _imageInfo->hMemDC; }
	inline int getMaxFrameX(void) { return _imageInfo->maxFrameX; }
	inline int getHeight(void) { return _imageInfo->height; }

	inline int getFrameWidth(void) { return _imageInfo->frameWidth; }
	inline int getFrameHeight(void) { return _imageInfo->frameHeight; }

	GImage();
	~GImage() {}
};

