#pragma once
#include "SingletonBase.h"

class RandomFuntion : public SingletonBase <RandomFuntion>
{
public:
	RandomFuntion()
	{
		// API 공유 함수 GetTickCount: OS가 부팅된 후의 경과 시간을 체크 (밀리세컨)
		// 규모가 크지 않은 난수에 쓰면 좋음
		srand(GetTickCount64());
	}
	~RandomFuntion() {}

	HRESULT init() { return S_OK; }
	inline int getInt(int num)
	{
		return rand() % num;
	}

	inline int getFromIntTo(int fromNum, int toNum)
	{
		return rand() % (toNum - fromNum + 1) + fromNum;
	}

	float getFloat(void)
	{
		return (float)rand() / (float)RAND_MAX;
	}

	float getFloat(float num)
	{
		return (float)rand() / (float)RAND_MAX * num;
	}

	float getFromFloatTo(float fromNum, float toNum)
	{
		float rnd = (float)rand() / (float)RAND_MAX;

		return (rnd * (toNum - fromNum) + fromNum);
	}
};