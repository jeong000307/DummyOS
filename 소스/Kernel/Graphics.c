// 나중에 비디오 드라이버로 이전할 계획

#include "Graphics.h"

STATIC  SCREEN  screen;

SCREEN*
DUMMYAPI GetScreen(
    VOID
)   {
    return &screen;
}

STATUS
DUMMYAPI InitializeScreen(
    IN CONST    GRAPHICS_INFORMATION* _graphicsInformation
)   {
    if (_graphicsInformation->horizontalResolution == 0 OR _graphicsInformation->verticalResolution == 0) {
        return ERR_INVALID_PARAMETER;
    }

    if ((VOID*)_graphicsInformation->framebuffer == NULL) {
        return ERR_DEVICE_ERROR;
    }

    screen.horizontalResolution = (UINTN)_graphicsInformation->horizontalResolution;
    screen.verticalResolution = (UINTN)_graphicsInformation->verticalResolution;

    screen.frameBuffer = (BYTE*)_graphicsInformation->framebuffer;
    screen.screenBuffer = AllocateMemory(screen.horizontalResolution * screen.verticalResolution * 4ull * sizeof(BYTE));

    SetMemory(screen.screenBuffer, 0, screen.horizontalResolution * screen.verticalResolution * 4ull * sizeof(BYTE));

    screen.WriteBuffer = (_graphicsInformation->pixelFormat == pixelRedGreenBlueReserved8BitPerColor)? _WriteBufferRGB: _WriteBufferBGR;
    
    screen.GetPixelAddress = _GetPixelAddress;
    screen.Refresh = _Refresh;

    screen.Refresh(&screen);

    return SUCCESS;
}

STATIC VOID
DUMMYAPI _WriteBufferRGB(
    IN CONST	SCREEN*     _this,
    IN CONST	UINTN		_x,
    IN CONST	UINTN		_y,
    IN CONST	PIXEL_COLOR	_color
)   {
    BYTE* pixel = _this->GetPixelAddress(_this, _x, _y);

    pixel[0] = _color.red;
    pixel[1] = _color.green;
    pixel[2] = _color.blue;
}

STATIC VOID
DUMMYAPI _WriteBufferBGR(
    IN CONST	SCREEN*     _this,
    IN CONST	UINTN		_x,
    IN CONST	UINTN		_y,
    IN CONST	PIXEL_COLOR _color
)   {
    BYTE* pixel = _this->GetPixelAddress(_this, _x, _y);

    pixel[0] = _color.blue;
    pixel[1] = _color.green;
    pixel[2] = _color.red;
}

STATIC BYTE*
DUMMYAPI _GetPixelAddress(
    IN CONST	SCREEN* _this,
    IN CONST	UINTN   _x,
    IN CONST	UINTN   _y
)   {
    return _this->screenBuffer + 4 * sizeof(BYTE) * (_this->horizontalResolution * _y + _x);
}

STATIC VOID
DUMMYAPI _Refresh(
    IN CONST	SCREEN* _this
)   {
    CONST UINTN screenSize = (UINTN)_this->horizontalResolution * (UINTN)_this->verticalResolution * sizeof(BYTE) * 4;
    CopyMemory(_this->screenBuffer, _this->frameBuffer, screenSize);

    return;
}