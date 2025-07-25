#pragma once
#include "SingletonBase.h"
//#include "GImage.h"

class GImage;

class ImageManager : public SingletonBase<ImageManager>
{
private:
    // GImage 포인터를 문자열 키와 함께 저장하는 map
    typedef std::map<std::string, GImage*> mapImageList;
    typedef std::map<std::string, GImage*> ::iterator mapImageIter;
private:
    mapImageList _mImageList;

public:

    HRESULT init(void);
    void release(void); // 모든 이미지 리소스를 해제

    GImage* addImage(std::string strKey, const char* fileName, int width, int height, bool isTrans = false, COLORREF transColor = RGB(0, 0, 0));
    // 프레임 이미지를 추가하는 함수
    GImage* addFrameImage(std::string strKey, const char* fileName, int width, int height, int maxFrameX, int maxFrameY, bool isTrans = false, COLORREF transColor = RGB(0, 0, 0));
    // 이미지를 찾아 포인터를 반환하는 함수
    GImage* findImage(std::string strKey);

    // 모든 이미지를 삭제
    void deleteAll();

    // 키 값으로 프레임 렌더링을 요청하는 함수
    void frameRender(std::string strKey, HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY);
    
    void loopRender(std::string strKey, HDC hdc, const LPRECT drawArea, int offSetX, int offSetY);

    ImageManager();
};