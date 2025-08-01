#pragma once
#pragma warning(disable : 4005)

#include <SDKDDKver.h>
#define WIN32_LEAN_AND_MEAN   
#include <Windows.h>

// C 런타임 헤더 파일
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <time.h>
#include <tchar.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

// C++ 런타임 헤더 파일
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
#include "ImageManager.h"
#include "HurdleManager.h"
#include "ProgressBar.h"
#include "EffectManager.h"
#include "SoundManager.h"

#define RND RandomFuntion::getSingleton()
#define KEYMANAGER KeyManager::getSingleton()
#define IMAGEMANAGER ImageManager::getSingleton()
#define EFFECTMANAGER EffectManager::getSingleton()
#define SOUNDMANAGER SoundManager::getSingleton()

// # 매크로 (윈도우창 초기화) #

#define WINNAME			(LPTSTR)(TEXT("Windows API"))
#define WINSTART_X		100
#define WINSTART_Y		1
#define WINSIZE_X		1067
#define WINSIZE_Y		600
#define WINSTYLE		WS_CAPTION | WS_SYSMENU

#define SAFE_DELETE(p)			{if(p) {delete (p); (p) = nullptr;}}
#define SAFE_DELETE_ARRAY(p)	{if(p) {delete[] (p); (p) = nullptr;}}
#define SAFE_RELEASE(p)			{if(p) {(p)->release(); (p) = nullptr;}

// static
extern HINSTANCE	_hInstance;
extern HWND			_hWnd;
extern POINT		_ptMouse;