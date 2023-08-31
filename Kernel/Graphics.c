#include "Graphics.h"

static SCREEN screen;

SCREEN* GetScreen(void) {
    return &screen;
}

code InitializeScreen(
  const struct FrameBufferConfiguration* frameBufferConfiguration) {
    if (frameBufferConfiguration->horizontalResolution == 0 or frameBufferConfiguration->verticalResolution == 0) {
        return INVALID_PARAMETER;
    }

    if (frameBufferConfiguration->frameBuffer == NULL) {
        return MEMORY_ERROR;
    }

    screen.horizontalResolution = (size)frameBufferConfiguration->horizontalResolution;
    screen.verticalResolution = (size)frameBufferConfiguration->verticalResolution;

    screen.frameBuffer = frameBufferConfiguration->frameBuffer;
    screen.screenBuffer = AllocateMemory(screen.horizontalResolution * screen.verticalResolution * 4ull * sizeof(byte));

    SetMemory(screen.screenBuffer, 0, screen.horizontalResolution * screen.verticalResolution * 4ull * sizeof(byte));

    screen.WriteBuffer = frameBufferConfiguration->pixelFormat == pixelRGBReserved8BitPerColor? __WriteBufferRGB: __WriteBufferBGR;
    
    screen.GetPixelAddress = __GetPixelAddress;
    screen.Refresh = __Refresh;

    screen.Refresh(&screen);

    return SUCCESS;
}

static void __WriteBufferRGB(
  const SCREEN*           this,
  const size              x,
  const size              y,
  const struct PixelColor color) {
    byte* pixel = this->GetPixelAddress(this, x, y);

    pixel[0] = color.red;
    pixel[1] = color.green;
    pixel[2] = color.blue;
}

static void __WriteBufferBGR(
  const SCREEN*           this,
  const size              x,
  const size              y,
  const struct PixelColor color) {
    byte* pixel = this->GetPixelAddress(this, x, y);

    pixel[0] = color.blue;
    pixel[1] = color.green;
    pixel[2] = color.red;
}

static byte* __GetPixelAddress(
  const SCREEN* this,
  const size    x,
  const size    y) {
    return this->screenBuffer + 4 * sizeof(byte) * (this->horizontalResolution * y + x);
}

static void __Refresh(
  const SCREEN* this) {
    const size screenSize = (size)this->horizontalResolution * (size)this->verticalResolution * sizeof(byte) * 4;
    CopyMemory(this->screenBuffer, this->frameBuffer, screenSize);

    return;
}