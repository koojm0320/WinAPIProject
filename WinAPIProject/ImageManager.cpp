#include "Stdafx.h"
#include "ImageManager.h"
#include "GImage.h"

ImageManager::ImageManager() {}

HRESULT ImageManager::init(void)
{
    return S_OK;
}

void ImageManager::release(void)
{
    this->deleteAll();
}

// 프레임 이미지를 생성하고 map에 추가
GImage* ImageManager::addFrameImage(std::string strKey, const char* fileName, int width, int height, int maxFrameX, int maxFrameY, bool isTrans, COLORREF transColor)
{
    GImage* img = findImage(strKey);
    if (img) return img; // 이미 존재하면 새로 만들지 않고 반환

    img = new GImage;
    if (FAILED(img->init(fileName, width, height, maxFrameX, maxFrameY, isTrans, transColor)))
    {
        SAFE_DELETE(img);
        return nullptr;
    }

    _mImageList.insert(make_pair(strKey, img));
    return img;
}

// 중요: 이미지를 삭제하지 않고, 찾아서 포인터를 반환하도록 수정
GImage* ImageManager::findImage(std::string strKey)
{
    mapImageIter key = _mImageList.find(strKey);
    if (key != _mImageList.end())
    {
        return key->second; // 찾은 이미지의 포인터 반환
    }
    return nullptr;
}

void ImageManager::deleteAll()
{
    for (auto& image : _mImageList)
    {
        if (image.second)
        {
            image.second->release();
            SAFE_DELETE(image.second);
        }
    }
    _mImageList.clear();
}

// ImageManager를 통해 프레임 렌더링 요청
void ImageManager::frameRender(std::string strKey, HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY)
{
    GImage* img = findImage(strKey);
    if (img)
    {
        img->frameRender(hdc, destX, destY, currentFrameX, currentFrameY);
    }
}

void ImageManager::loopRender(std::string strKey, HDC hdc, const LPRECT drawArea, int offSetX, int offSetY)
{
    GImage* img = findImage(strKey);
    if (img) img->loopRender(hdc, drawArea, offSetX, offSetY);
}
