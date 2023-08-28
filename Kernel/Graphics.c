#include "Graphics.h"

static SCREEN screen;

SCREEN* GetScreen(void) {
    return &screen;
}

code InitializeScreen(
  IN const struct FRAME_BUFFER_CONFIG* frameBufferConfiguration) {
    if (frameBufferConfiguration->pixelsPerScanLine == 0 or frameBufferConfiguration->horizontalResolution == 0 or frameBufferConfiguration->verticalResolution == 0) {
        return INVALID_PARAMETER;
    }

    if (frameBufferConfiguration->frameBuffer == NULL) {
        return MEMORY_ERROR;
    }

    screen.frameBuffer = frameBufferConfiguration->frameBuffer;

    screen.pixelsPerScanLine = frameBufferConfiguration->pixelsPerScanLine;
    screen.horizontalResolution = frameBufferConfiguration->horizontalResolution;
    screen.verticalResolution = frameBufferConfiguration->verticalResolution;

    SetMemory(screen.frameBuffer, 0, (size)screen.horizontalResolution * (size)screen.verticalResolution * sizeof(byte) * 4);

    screen.WritePixel = frameBufferConfiguration->pixelFormat == pixelRGBReserved8BitPerColor? __WritePixelRGB: __WritePixelBGR;
    
    screen.GetPixelAddress = __GetPixelAddress;

    return SUCCESS;
}

static void __WritePixelRGB(
  IN const SCREEN*           this,
  IN size                    x,
  IN size                    y,
  IN const struct PixelColor color) {
    byte* pixel = this->GetPixelAddress(this, x, y);

    pixel[0] = color.red;
    pixel[1] = color.green;
    pixel[2] = color.blue;
}

static void __WritePixelBGR(
    IN const SCREEN*           this,
    IN size                    x,
    IN size                    y,
    IN const struct PixelColor color) {
    byte* pixel = this->GetPixelAddress(this, x, y);

    pixel[0] = color.blue;
    pixel[1] = color.green;
    pixel[2] = color.red;
}

static byte* __GetPixelAddress(
  IN const SCREEN* this,
  IN size          x,
  IN size          y) {
    return this->frameBuffer + 4 * sizeof(byte) * (this->pixelsPerScanLine * y + x);
}