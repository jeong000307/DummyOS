// 나중에 시스템 애플리케이션으로 이전할 계획

#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "Error.h"
#include "Graphics.h"
#include "Memory.h"
#include "String.h"
#include "Type.h"

#define CONSOLE_ROW		25
#define CONSOLE_COLUMN	80

typedef	struct _CONSOLE	CONSOLE;

typedef struct {
	UINTN	x;
	UINTN	y;
} CURSOR;

struct _CONSOLE {
	UINTN		rows;
	UINTN		columns;

	CURSOR		cursor;
	PIXEL_COLOR	foregroundColor;
	PIXEL_COLOR	backgroundColor;

	SCREEN*		screen;

	VOID		(DUMMYAPI* Print)(
					IN			CONSOLE*	_this,
					IN CONST	BYTE*		_string,
					...
				);
};

CONSOLE* 
DUMMYAPI GetSystemConsole(
	VOID
);

STATUS 
DUMMYAPI InitializeSystemConsole(
	IN	SCREEN*	_screen
);

STATIC VOID
DUMMYAPI _Print(
	IN			CONSOLE*	_this,
	IN CONST	BYTE*		_string,
	...
);

STATIC STATUS
DUMMYAPI WriteASCII(
	IN CONST	SCREEN*		_screen,
	IN CONST	UINTN		_x,
	IN CONST	UINTN		_y,
	IN CONST	BYTE		_character,
	IN CONST	PIXEL_COLOR	_color
);

#endif