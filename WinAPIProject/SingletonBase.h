#pragma once

template <class T>

class SingletonBase
{
private:
	static T* singleton;

public:
	static T* getSingleton(void);
	void releaseSingleton(void);

	SingletonBase() {}
	~SingletonBase() {}
};

template<typename T>
T* SingletonBase<T>::singleton = 0;

template<typename T>
T* SingletonBase<T>::getSingleton(void)
{
	// ����� �ڵ�
	if (!singleton) singleton = new T;

	return singleton;
}

template<typename T>
void SingletonBase<T>::releaseSingleton(void)
{
	// ����� �ڵ�
	if (singleton)
	{
		delete singleton;
		singleton = 0;
	}
}

