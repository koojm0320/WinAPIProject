#include "GImage.h"
#include "Stdafx.h"


GImage::GImage() : _imageInfo(nullptr),
_fileName(nullptr),
_isTrans(false),
_transColor(RGB(0, 0, 0))
{
    // !!
}

HRESULT GImage::init(int width, int height)
{
    // ���ʱ�ȭ ���ڴ�, �̹��� ������ ������ ������
    if (_imageInfo != nullptr) this->release();

    HDC hdc = GetDC(_hWnd);

    _imageInfo = new IMAGE_INFO;
    _imageInfo->loadType = LOAD_EMPTY;
    _imageInfo->resID = 0;
    _imageInfo->hMemDC = CreateCompatibleDC(hdc);
    _imageInfo->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, width, height);
    _imageInfo->hOBIT = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
    _imageInfo->width = width;
    _imageInfo->height = height;

    _fileName = nullptr;

    _isTrans = false;
    _transColor = RGB(0, 0, 0);

    if (_imageInfo->hBit == 0)
    {
        release();
        return E_FAIL;
    }

    ReleaseDC(_hWnd, hdc);


    return S_OK;
}

HRESULT GImage::init(const char* fileName, int width, int height, bool isTrans, COLORREF transColor)
{
    if (_imageInfo != nullptr) this->release();

    HDC hdc = GetDC(_hWnd);

    _imageInfo = new IMAGE_INFO;
    _imageInfo->loadType = LOAD_FILE;
    _imageInfo->resID = 0;
    _imageInfo->hMemDC = CreateCompatibleDC(hdc);
    _imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
    _imageInfo->hOBIT = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
    _imageInfo->width = width;
    _imageInfo->height = height;

    // �ܺο��� �Է¹��� ���� �̸�
    // �Է¹��� ���� �̸� �������� �Ҵ�
    int len = strlen(fileName);

    // len + 1�� �ϴ� ���� : null�� ���� + �̸��� ������ �� ȭ��Ʈ �����̽��� ���� ������ ������ ����ؼ� �߰�
    _fileName = new char[len + 1];
    strcpy_s(_fileName, len + 1, fileName);

    _isTrans = isTrans;
    _transColor = transColor;

    if (_imageInfo->hBit == 0)
    {
        release();
        return E_FAIL;
    }

    ReleaseDC(_hWnd, hdc);

    return S_OK;
}

void GImage::setTransColor(bool isTrans, COLORREF transColor)
{
    _isTrans = isTrans;
    _transColor = transColor;
}

void GImage::release(void)
{
    // �̹��� ������ �ִٸ�
    if (_imageInfo)
    {
        // ���� -> �̹���
        SelectObject(_imageInfo->hMemDC, _imageInfo->hOBIT);
        DeleteObject(_imageInfo->hBit);
        DeleteDC(_imageInfo->hMemDC);

        // ���� -> ������ (��۸� ����)
        SAFE_DELETE(_imageInfo);
        SAFE_DELETE_ARRAY(_fileName);

        // �ʱ�ȭ
        _isTrans = false;
        _transColor = RGB(0, 0, 0);
    }
}


// 0, 0
void GImage::render(HDC hdc)
{
    if (_isTrans)
    {
        // GdiTransparentBlt(): Ư�� ������ ���� ���縦 ������.
        GdiTransparentBlt
        (
            hdc,
            0, 0,
            _imageInfo->width,
            _imageInfo->height,
            _imageInfo->hMemDC,
            0, 0,
            _imageInfo->width,
            _imageInfo->height,
            _transColor
        );
    }
    else
    {
        // BitBlt(): DC ���� �������� ���� ��Ӻ��� ����
        // SRCCOPY: �ҽ� ���� -> ��� ������ ����
        BitBlt(hdc, 0, 0, _imageInfo->width, _imageInfo->height, _imageInfo->hMemDC, 0, 0, SRCCOPY);
    }
}

// ��ǥ ����
void GImage::render(HDC hdc, int destX, int destY)
{
    if (_isTrans)
    {
        GdiTransparentBlt
        (
            hdc,
            destX, destY,
            _imageInfo->width,
            _imageInfo->height,
            _imageInfo->hMemDC,
            0, 0,
            _imageInfo->width,
            _imageInfo->height,
            _transColor
        );
    }
    else
    {
        BitBlt(hdc, destX, destY, _imageInfo->width, _imageInfo->height, _imageInfo->hMemDC, 0, 0, SRCCOPY);
    }
}
