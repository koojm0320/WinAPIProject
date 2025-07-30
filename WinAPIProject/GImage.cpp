#include "Stdafx.h"
#include "GImage.h"


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

HRESULT GImage::init(const char* fileName, int width, int height, int maxFrameX, int maxFrameY, bool isTrans, COLORREF transColor)
{
    if (_imageInfo != nullptr) this->release();

    HDC hdc = GetDC(_hWnd);

    _imageInfo = new IMAGE_INFO;
    _imageInfo->loadType = LOAD_FILE;
    _imageInfo->hMemDC = CreateCompatibleDC(hdc);
    _imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
    _imageInfo->hOBIT = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
    _imageInfo->width = width;
    _imageInfo->height = height;

    _imageInfo->currentFrameX = 0;
    _imageInfo->currentFrameY = 0;
    _imageInfo->maxFrameX = maxFrameX - 1;
    _imageInfo->maxFrameY = maxFrameY - 1;
    _imageInfo->frameWidth = width / maxFrameX;
    _imageInfo->frameHeight = height / maxFrameY;

    int len = strlen(fileName);
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
    if (_imageInfo)
    {

        SelectObject(_imageInfo->hMemDC, _imageInfo->hOBIT);
        DeleteObject(_imageInfo->hBit);
        DeleteDC(_imageInfo->hMemDC);

        SAFE_DELETE(_imageInfo);
        SAFE_DELETE_ARRAY(_fileName);

        _isTrans = false;
        _transColor = RGB(0, 0, 0);
    }
}


void GImage::render(HDC hdc)
{
    if (_isTrans)
    {
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
        BitBlt(hdc, 0, 0, _imageInfo->width, _imageInfo->height, _imageInfo->hMemDC, 0, 0, SRCCOPY);
    }
}

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

void GImage::render(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight)
{
    if (_isTrans)
    {
        GdiTransparentBlt
        (
            hdc,
            destX, destY,
            sourWidth,
            sourHeight,
            _imageInfo->hMemDC,
            sourX, sourY,
            sourWidth,
            sourHeight,
            _transColor
        );
    }
    else
    {
        BitBlt(hdc, destX, destY, sourWidth, sourHeight, _imageInfo->hMemDC, sourX, sourY, SRCCOPY);
    }
}


void GImage::frameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY)
{
    // ���� ������ ��ġ ������Ʈ
    _imageInfo->currentFrameX = currentFrameX;
    _imageInfo->currentFrameY = currentFrameY;

    // �߶� �ҽ�(�̹��� ����)�� �»�� ��ǥ ���
    int sourceX = _imageInfo->frameWidth * currentFrameX;
    int sourceY = _imageInfo->frameHeight * currentFrameY;

    // ����� ó���� �ʿ��� ���
    if (_isTrans)
    {
        GdiTransparentBlt(
            hdc,							
            destX, destY,					
            _imageInfo->frameWidth,			
            _imageInfo->frameHeight,		
            _imageInfo->hMemDC,				
            sourceX, sourceY,				
            _imageInfo->frameWidth,			
            _imageInfo->frameHeight,		
            _transColor);					
    }
    else // ����� ó���� �ʿ� ���� ���
    {
        BitBlt(
            hdc, destX, destY,
            _imageInfo->frameWidth, _imageInfo->frameHeight,
            _imageInfo->hMemDC,
            sourceX, sourceY,
            SRCCOPY);
    }
}

void GImage::loopRender(HDC hdc, const LPRECT drawArea, int offSetX, int offSetY)
{
    if (offSetX < 0) offSetX = _imageInfo->width + (offSetX % _imageInfo->width);
    if (offSetY < 0) offSetY = _imageInfo->height + (offSetY % _imageInfo->height);

    RECT rcSour;
    int sourWidth;
    int sourHeight;

    RECT rcDest;

    int drawAreaX = drawArea->left;
    int drawAreaY = drawArea->top;
    int drawAreaW = drawArea->right - drawArea->left;
    int drawAreaH = drawArea->bottom - drawArea->top;

    // Y(����) ���� ����
    for (int y = 0; y < drawAreaH; y += sourHeight)
    {
        // �ҽ� ������ ���̸� ���
        rcSour.top = (y + offSetY) % _imageInfo->height;
        rcSour.bottom = _imageInfo->height;
        sourHeight = rcSour.bottom - rcSour.top;

        // �ҽ� ������ �׸��� ȭ�� ������ �Ѿ (ȭ�� ������ �Ѿ)
        if (y + sourHeight > drawAreaH)
        {
            rcSour.bottom -= (y + sourHeight) - drawAreaH;
            sourHeight = rcSour.bottom - rcSour.top;
        }

        // �׷����� ����
        rcDest.top = y + drawAreaY;
        rcDest.bottom = rcDest.top + sourHeight;

        // X(����) ���� ����
        for (int x = 0; x < drawAreaW; x += sourWidth)
        {
            // �ҽ� ������ ���̸� ���
            rcSour.left = (x + offSetX) % _imageInfo->width;
            rcSour.right = _imageInfo->width;
            sourWidth = rcSour.right - rcSour.left;

            // �ҽ� ������ �׸��� ȭ�� ������ �Ѿ (ȭ�� ������ �Ѿ)
            if (x + sourWidth > drawAreaW)
            {
                rcSour.right -= (x + sourWidth) - drawAreaW;
                sourWidth = rcSour.right - rcSour.left;
            }

            // �׷����� ����
            rcDest.left = x + drawAreaX;
            rcDest.right = rcDest.left + sourWidth;


            render(hdc, rcDest.left, rcDest.top, rcSour.left, rcSour.top, sourWidth, sourHeight);
        } // <- end of forX
    } // <- end of forY


}


void GImage::alphaRender(HDC hdc, int destX, int destY, BYTE alpha)
{
    BLENDFUNCTION bf;
    bf.BlendOp = AC_SRC_OVER;
    bf.BlendFlags = 0;
    bf.SourceConstantAlpha = alpha;
    bf.AlphaFormat = 0; // AC_SRC_ALPHA

    GdiAlphaBlend(hdc, destX, destY, _imageInfo->width, _imageInfo->height,
        _imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, bf);
}

void GImage::alphaFrameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY, BYTE alpha)
{
    if (!_isTrans)
    {
        BLENDFUNCTION bf;
        bf.BlendOp = AC_SRC_OVER;
        bf.BlendFlags = 0;
        bf.SourceConstantAlpha = alpha;
        bf.AlphaFormat = 0;

        int sourceX = _imageInfo->frameWidth * currentFrameX;
        int sourceY = _imageInfo->frameHeight * currentFrameY;

        GdiAlphaBlend(hdc,
            destX, destY,
            _imageInfo->frameWidth, _imageInfo->frameHeight,
            _imageInfo->hMemDC,
            sourceX, sourceY,
            _imageInfo->frameWidth, _imageInfo->frameHeight,
            bf);
        return;
    }

    BITMAPINFO bmi;
    ZeroMemory(&bmi, sizeof(BITMAPINFO));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = _imageInfo->frameWidth;
    bmi.bmiHeader.biHeight = -_imageInfo->frameHeight;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    BYTE* pBits;
    HBITMAP hbmAlpha = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&pBits, NULL, 0);
    if (!hbmAlpha) return;

    HDC hdcAlpha = CreateCompatibleDC(hdc);
    HBITMAP hbmOldAlpha = (HBITMAP)SelectObject(hdcAlpha, hbmAlpha);

    int sourceX = _imageInfo->frameWidth * currentFrameX;
    int sourceY = _imageInfo->frameHeight * currentFrameY;
    BitBlt(hdcAlpha, 0, 0, _imageInfo->frameWidth, _imageInfo->frameHeight, _imageInfo->hMemDC, sourceX, sourceY, SRCCOPY);

    for (int y = 0; y < _imageInfo->frameHeight; ++y)
    {
        for (int x = 0; x < _imageInfo->frameWidth; ++x)
        {
            BYTE* pPixel = &pBits[(y * _imageInfo->frameWidth + x) * 4];
            COLORREF pixelColor = RGB(pPixel[2], pPixel[1], pPixel[0]); 

            if (pixelColor == _transColor)
            {
                pPixel[3] = 0;
            }
            else
            {

                pPixel[0] = (pPixel[0] * alpha) / 255; 
                pPixel[1] = (pPixel[1] * alpha) / 255; 
                pPixel[2] = (pPixel[2] * alpha) / 255; 
                pPixel[3] = alpha;
            }
        }
    }
    BLENDFUNCTION bf;
    bf.BlendOp = AC_SRC_OVER;
    bf.BlendFlags = 0;
    bf.SourceConstantAlpha = 255; 
    bf.AlphaFormat = AC_SRC_ALPHA;

    GdiAlphaBlend(hdc, destX, destY, _imageInfo->frameWidth, _imageInfo->frameHeight,
        hdcAlpha, 0, 0, _imageInfo->frameWidth, _imageInfo->frameHeight, bf);

    SelectObject(hdcAlpha, hbmOldAlpha);
    DeleteObject(hbmAlpha);
    DeleteDC(hdcAlpha);
}

void GImage::alphaRenderWithTransparency(HDC hdc, int destX, int destY, BYTE alpha)
{
    if (!_isTrans)
    {
        alphaRender(hdc, destX, destY, alpha);
        return;
    }


    BITMAPINFO bmi;
    ZeroMemory(&bmi, sizeof(BITMAPINFO));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = _imageInfo->width;
    bmi.bmiHeader.biHeight = -_imageInfo->height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    BYTE* pBits;
    HBITMAP hbmAlpha = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&pBits, NULL, 0);
    if (!hbmAlpha) return;

    HDC hdcAlpha = CreateCompatibleDC(hdc);
    HBITMAP hbmOldAlpha = (HBITMAP)SelectObject(hdcAlpha, hbmAlpha);


    BitBlt(hdcAlpha, 0, 0, _imageInfo->width, _imageInfo->height, _imageInfo->hMemDC, 0, 0, SRCCOPY);


    for (int y = 0; y < _imageInfo->height; ++y)
    {
        for (int x = 0; x < _imageInfo->width; ++x)
        {
            BYTE* pPixel = &pBits[(y * _imageInfo->width + x) * 4];
            COLORREF pixelColor = RGB(pPixel[2], pPixel[1], pPixel[0]); 

            if (pixelColor == _transColor)
            {
                pPixel[3] = 0;
            }
            else
            {
                pPixel[3] = 255;
            }
        }
    }

    BLENDFUNCTION bf;
    bf.BlendOp = AC_SRC_OVER;
    bf.BlendFlags = 0;
    bf.SourceConstantAlpha = alpha;     
    bf.AlphaFormat = AC_SRC_ALPHA;

    GdiAlphaBlend(hdc, destX, destY, _imageInfo->width, _imageInfo->height,
        hdcAlpha, 0, 0, _imageInfo->width, _imageInfo->height, bf);

    SelectObject(hdcAlpha, hbmOldAlpha);
    DeleteObject(hbmAlpha);
    DeleteDC(hdcAlpha);
}
