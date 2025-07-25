#pragma once
#include "SingletonBase.h"
//#include "GImage.h"

class GImage;

class ImageManager : public SingletonBase<ImageManager>
{
private:
    // GImage �����͸� ���ڿ� Ű�� �Բ� �����ϴ� map
    typedef std::map<std::string, GImage*> mapImageList;
    typedef std::map<std::string, GImage*> ::iterator mapImageIter;
private:
    mapImageList _mImageList;

public:

    HRESULT init(void);
    void release(void); // ��� �̹��� ���ҽ��� ����

    GImage* addImage(std::string strKey, const char* fileName, int width, int height, bool isTrans = false, COLORREF transColor = RGB(0, 0, 0));
    // ������ �̹����� �߰��ϴ� �Լ�
    GImage* addFrameImage(std::string strKey, const char* fileName, int width, int height, int maxFrameX, int maxFrameY, bool isTrans = false, COLORREF transColor = RGB(0, 0, 0));
    // �̹����� ã�� �����͸� ��ȯ�ϴ� �Լ�
    GImage* findImage(std::string strKey);

    // ��� �̹����� ����
    void deleteAll();

    // Ű ������ ������ �������� ��û�ϴ� �Լ�
    void frameRender(std::string strKey, HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY);
    
    void loopRender(std::string strKey, HDC hdc, const LPRECT drawArea, int offSetX, int offSetY);

    ImageManager();
};