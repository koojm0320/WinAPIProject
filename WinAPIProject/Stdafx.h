#pragma once
#pragma warning(disable : 4005)

#include <SDKDDKver.h>
#define WIN32_LEAN_AND_MEAN   
#include <Windows.h>

// C ��Ÿ�� ��� ����
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <time.h>
#include <tchar.h>

// C++ ��Ÿ�� ��� ����
#include <iostream>
#include <chrono>
#include <vector>
#include <map>
#include <unordered_map>
#include <random>
#include <cassert>
#include <bitset>
#include <string>

#include "CommonMacroFuntion.h"
#include "RandomFuntion.h"
#include "KeyManager.h"

#define RND RandomFuntion::getSingleton()
#define KEYMANAGER KeyManager::getSingleton()

// # ��ũ�� (������â �ʱ�ȭ) #

#define WINNAME			(LPTSTR)(TEXT("Windows API"))
#define WINSTART_X		400
#define WINSTART_Y		1
#define WINSIZE_X		1600
#define WINSIZE_Y		900
#define WINSTYLE		WS_CAPTION | WS_SYSMENU

#define SAFE_DELETE(p)			{if(p) {delete (p); (p) = nullptr;}}
#define SAFE_DELETE_ARRAY(p)	{if(p) {delete[] (p); (p) = nullptr;}}
#define SAFE_RELEASE(p)			{if(p) {(p)->release(); (p) = nullptr;}

// static
extern HINSTANCE	_hInstance;
extern HWND			_hWnd;
extern POINT		_ptMouse;