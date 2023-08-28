#include "Graphics.h"

static SCREEN screen;

SCREEN* GetScreen(void) {
    return &screen;
}

code InitializeScreen(
  IN const struct FRAME_BUFFER_CONFIG* frameBufferConfiguration) {
    if (frameBufferConfiguration->pixelsPerScanLine == 0
      or frameBufferConfiguration->horizontalResolution == 0
      or frameBufferConfiguration->verticalResolution == 0) {
        return INVALID_PARAMETER;
    }

    if (frameBufferConfiguration->frameBuffer == NULL) {
        return MEMORY_ERROR;
    }

    screen.frameBufferConfiguration.pixelFormat = frameBufferConfiguration->pixelFormat;
    screen.frameBufferConfiguration.frameBuffer = frameBufferConfiguration->frameBuffer;

    screen.frameBufferConfiguration.pixelsPerScanLine = frameBufferConfiguration->pixelsPerScanLine;
    screen.frameBufferConfiguration.horizontalResolution = frameBufferConfiguration->horizontalResolution;
    screen.frameBufferConfiguration.verticalResolution = frameBufferConfiguration->verticalResolution;

    SetMemory(
      screen.frameBufferConfiguration.frameBuffer,
      0, 
      (size)screen.frameBufferConfiguration.horizontalResolution * (size)screen.frameBufferConfiguration.verticalResolution * sizeof(byte) * 4);

    if (frameBufferConfiguration->pixelFormat == pixelRGBReserved8BitPerColor) {
        screen.WritePixel = __WritePixelRGB;
    }
    else {
        screen.WritePixel = __WritePixelBGR;
    }
    
    screen.GetPixelAddress = __GetPixelAddress;

    return SUCCESS;
}

static code __WritePixelRGB(
  IN OUT const SCREEN*           this,
  IN     size                    x,
  IN     size                    y,
  IN     const struct PixelColor color) {
    byte* pixel = this->GetPixelAddress(
      this, 
      x, 
      y);

    pixel[0] = color.red;
    pixel[1] = color.green;
    pixel[2] = color.blue;

    return SUCCESS;
}

static code __WritePixelBGR(
    IN OUT const SCREEN*           this,
    IN     size                    x,
    IN     size                    y,
    IN     const struct PixelColor color) {
    byte* pixel = this->GetPixelAddress(
      this, 
      x, 
      y);

    pixel[0] = color.blue;
    pixel[1] = color.green;
    pixel[2] = color.red;

    return SUCCESS;
}

static byte* __GetPixelAddress(
  IN const SCREEN* this,
  IN size          x,
  IN size          y) {
    return this->frameBufferConfiguration.frameBuffer + 4 * sizeof(byte) * (this->frameBufferConfiguration.pixelsPerScanLine * y + x);
}