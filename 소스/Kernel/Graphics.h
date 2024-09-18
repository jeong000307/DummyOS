// 나중에 비디오 드라이버로 이전할 계획

#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "Error.h"
#include "Memory.h"
#include "SystemConfiguration.h"
#include "Type.h"

typedef struct _SCREEN SCREEN;

typedef struct {
	BYTE	red;
	BYTE	green;
	BYTE	blue;
} PIXEL_COLOR;

struct _SCREEN {
	UINTN	horizontalResolution;
	UINTN	verticalResolution;

	BYTE*	screenBuffer;
	BYTE*	frameBuffer;

	VOID	(DUMMYAPI* WriteBuffer)(
				IN CONST	SCREEN*			_this,
				IN CONST	UINTN			_x,
				IN CONST	UINTN			_y,
				IN CONST	PIXEL_COLOR		_color
			);

	BYTE*	(DUMMYAPI* GetPixelAddress)(
				IN CONST	SCREEN*	_this,
				IN CONST	UINTN	_x,
				IN CONST	UINTN	_y
			);

	VOID	(DUMMYAPI* Refresh)(
				IN CONST	SCREEN*	_this
			);
};

SCREEN* 
DUMMYAPI GetScreen(
	VOID
);

STATUS 
DUMMYAPI InitializeScreen(
	IN CONST	GRAPHICS_INFORMATION*	_graphicsInformation
);

STATIC VOID 
DUMMYAPI _WriteBufferRGB(
	IN CONST	SCREEN*		_this,
	IN CONST	UINTN		_x,
	IN CONST	UINTN		_y,
	IN CONST	PIXEL_COLOR	_color
);

STATIC VOID 
DUMMYAPI _WriteBufferBGR(
	IN CONST	SCREEN*		_this,
	IN CONST	UINTN		_x,
	IN CONST	UINTN		_y,
	IN CONST	PIXEL_COLOR _color
);

STATIC BYTE* 
DUMMYAPI _GetPixelAddress(
	IN CONST	SCREEN*	_this,
	IN CONST	UINTN   _x,
	IN CONST	UINTN   _y
);

STATIC VOID 
DUMMYAPI _Refresh(
	IN CONST	SCREEN*	this
);

#endif