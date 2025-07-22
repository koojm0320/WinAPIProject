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
	// 规绢利 内爹
	if (!singleton) singleton = new T;

	return singleton;
}

template<typename T>
void SingletonBase<T>::releaseSingleton(void)
{
	// 规绢利 内爹
	if (singleton)
	{
		delete singleton;
		singleton = 0;
	}
}

