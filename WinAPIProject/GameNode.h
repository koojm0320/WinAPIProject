#pragma once
#include "GImage.h"

class GameNode
{
private:
	GImage* _backBuffer;
	void setBackBuffer(void);

public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(HDC hdc);

	GImage* getBackBuffer(void) { return _backBuffer; }
	LRESULT mainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

	GameNode();
	virtual ~GameNode();
};

