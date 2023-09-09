// 나중에 비디오 드라이버로 이전할 계획

#pragma once

#include "Error.h"
#include "Memory.h"
#include "SystemConfiguration.h"
#include "Type.h"

enum PixelFormat {
    pixelRGBReserved8BitPerColor,
    pixelBGRReserved8BitPerColor
};

struct PixelColor {
    byte red;
    byte green;
    byte blue;
};

typedef struct __SCREEN SCREEN;

typedef void (*WRITE_BUFFER)(
  const SCREEN*           this,
  const size              x,
  const size              y,
  const struct PixelColor color);

typedef byte* (*GET_PIXEL_ADDRESS)(
  const SCREEN* this,
  const size    x,
  const size    y);

typedef void (*REFRESH)(
  const SCREEN* this);

struct __SCREEN {
    size              horizontalResolution;
    size              verticalResolution;

    byte*             screenBuffer;
    byte*             frameBuffer;

    WRITE_BUFFER      WriteBuffer;
    GET_PIXEL_ADDRESS GetPixelAddress;
    REFRESH           Refresh;
};

SCREEN* GetScreen(void);

code InitializeScreen(
  const struct FrameBufferConfiguration* frameBufferConfig);

static void __WriteBufferRGB(
  const SCREEN*           this,
  const size              x,
  const size              y,
  const struct PixelColor color);

static void __WriteBufferBGR(
  const SCREEN*           this,
  const size              x,
  const size              y,
  const struct PixelColor color);

static byte* __GetPixelAddress(
  const SCREEN* this,
  const size    x,
  const size    y);

static void __Refresh(
  const SCREEN* this);