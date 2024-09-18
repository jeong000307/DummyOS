// 나중에 시스템 애플리케이션으로 이전할 계획

#include "Console.h"
#include "AssemblyFunction.h"

EXTERN CONST    BYTE    fonts[];
STATIC          CONSOLE systemConsole;

CONSOLE*
DUMMYAPI GetSystemConsole(
	VOID
)   {
	return &systemConsole;
}

STATUS
DUMMYAPI InitializeSystemConsole(
	IN	SCREEN* _screen
)   {
	systemConsole.rows = CONSOLE_ROW;
	systemConsole.columns = CONSOLE_COLUMN;
	systemConsole.cursor = (CURSOR){0, 0};

	systemConsole.foregroundColor = (PIXEL_COLOR){255, 255, 255};
	systemConsole.backgroundColor = (PIXEL_COLOR){0, 0, 0};
	systemConsole.screen = _screen;

	systemConsole.Print = _Print;

	return SUCCESS;
}

STATIC VOID
DUMMYAPI _Print(
	IN 	        CONSOLE* _this,
	IN CONST	BYTE* _string,
	...
)   {
	UINTN   index;

	VA_LIST list;
	BYTE    copyBuffer[1000];

	VA_START(list, _string);

	for (index = 0; _string[index] != NUL; ++index) {
		if (_this->cursor.x >= _this->columns) {
			_this->cursor.x = 0;
			++_this->cursor.y;
		}

		if (_this->cursor.y >= _this->rows) {
			CopyMemory(_this->screen->GetPixelAddress(_this->screen, 0, 16), _this->screen->screenBuffer, _this->screen->horizontalResolution * (_this->screen->verticalResolution - 16) * 4 * sizeof(BYTE));
			SetMemory(_this->screen->GetPixelAddress(_this->screen, 0, (_this->rows - 1) * 16), 0, _this->screen->horizontalResolution * 16 * 4 * sizeof(BYTE));

			--_this->cursor.y;
			_this->cursor.x = 0;
		}

		if (_string[index] == '%') {
			++index;
			SetMemory(copyBuffer, NUL, 1000 * sizeof(BYTE));

			switch (_string[index]) {
				case 's':
					CopyString(VA_ARG(list, BYTE*), copyBuffer);
					_Print(_this, copyBuffer);
					break;
				case 'c':
					copyBuffer[0] = VA_ARG(list, BYTE);
					copyBuffer[1] = NUL;
					_Print(_this, copyBuffer);
					break;
				case 'd':
				case 'i':
					ConvertDecimalToString(copyBuffer, VA_ARG(list, INT64), TRUE);
					_Print(_this, copyBuffer);
					break;
				case 'u':
					ConvertDecimalToString(copyBuffer, VA_ARG(list, UINT64), FALSE);
					_Print(_this, copyBuffer);
					break;
				case 'x':
					ConvertHexadecimalToString(copyBuffer, VA_ARG(list, UINT64) & 0xFFFFFFFF, FALSE);
					_Print(_this, copyBuffer);
					break;
				case 'X':
					ConvertHexadecimalToString(copyBuffer, VA_ARG(list, UINT64) & 0xFFFFFFFF, TRUE);
					_Print(_this, copyBuffer);
					break;
				case 'p':
					ConvertHexadecimalToString(copyBuffer, VA_ARG(list, UINT64), TRUE);
					_Print(_this, copyBuffer);
					break;
				/*case 'f':
					copyBuffer = ConvertNumberToString('f', VA_ARG(list, double));
					__Print(_this, copyBuffer);
					break;*/
				default:
					_Print(_this, _string + index);
			}
		} else {
			switch (_string[index]) {
				case '\n':
					++_this->cursor.y;
					_this->cursor.x = 0;
					break;
				default:
					WriteASCII(_this->screen, 8 * _this->cursor.x, 16 * _this->cursor.y, _string[index], _this->foregroundColor);
					++_this->cursor.x;
			}
		}
	}

	VA_END(list);

	_this->screen->Refresh(_this->screen);
}

STATIC STATUS
DUMMYAPI WriteASCII(
	IN CONST	SCREEN*     _screen,
	IN CONST	UINTN		_x,
	IN CONST	UINTN		_y,
	IN CONST	BYTE		_character,
	IN CONST	PIXEL_COLOR	_color
)   {
			UINTN   dx, dy;
	CONST   BYTE*   font = fonts + 16ull * (ADDRESS)_character;

	for (dy = 0; dy < 16; ++dy) {
		for (dx = 0; dx < 8; ++dx) {
			if ((font[dy] << dx) & 0x80u) {
				_screen->WriteBuffer(_screen, _x + dx, _y + dy, _color);
			}
		}
	}

	return SUCCESS;
}
