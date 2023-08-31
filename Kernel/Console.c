#include "Console.h"
#include "AssemblyFunction.h"

extern const byte fonts[];

static CONSOLE systemConsole;

CONSOLE* GetSystemConsole(void) {
    return &systemConsole;
}

code InitializeSystemConsole(
  SCREEN* screen) {
    systemConsole.rows = CONSOLE_ROW;
    systemConsole.columns = CONSOLE_COLUMN;
    systemConsole.cursor = (struct Cursor){0, 0};

    systemConsole.foregroundColor = (struct PixelColor){255, 255, 255};
    systemConsole.backgroundColor = (struct PixelColor){0, 0, 0};
    systemConsole.screen = screen;

    systemConsole.Print = __Print;

    return SUCCESS;
}

static void __Print(
  CONSOLE*        this,
  const byte*     string,
  ...) {
    size    index;

    va_list list;
    byte    copyBuffer[1000];

    va_start(list, string);

    for (index = 0; string[index] != '\0'; ++index) {
        if (this->cursor.x >= this->columns) {
            this->cursor.x = 0;
            ++this->cursor.y;
        }

        if (this->cursor.y >= this->rows) {
            CopyMemory(this->screen->GetPixelAddress(this->screen, 0, 16), this->screen->screenBuffer, this->screen->horizontalResolution * (this->screen->verticalResolution - 16) * 4 * sizeof(byte));
            SetMemory(this->screen->GetPixelAddress(this->screen, 0, (this->rows - 1) * 16), 0, this->screen->horizontalResolution * 16 * 4 * sizeof(byte));

            --this->cursor.y;
            this->cursor.x = 0;
        }

        if (string[index] == '%') {
            ++index;
            SetMemory(copyBuffer, '\0', 1000 * sizeof(byte));

            switch (string[index]) {
                case 's':
                    CopyString(va_arg(list, byte*), copyBuffer);
                    __Print(this, copyBuffer);
                    break;
                case 'c':
                    copyBuffer[0] = va_arg(list, byte);
                    copyBuffer[1] = '\0';
                    __Print(this, copyBuffer);
                    break;
                case 'd':
                case 'i':
                    ConvertDecimalToString(copyBuffer, va_arg(list, int64), true);
                    __Print(this, copyBuffer);
                    break;
                case 'u':
                    ConvertDecimalToString(copyBuffer, va_arg(list, uint64), false);
                    __Print(this, copyBuffer);
                    break;
                case 'x':
                    ConvertHexadecimalToString(copyBuffer, va_arg(list, uint64) & 0xFFFFFFFF, false);
                    __Print(this, copyBuffer);
                    break;
                case 'X':
                    ConvertHexadecimalToString(copyBuffer, va_arg(list, uint64) & 0xFFFFFFFF, true);
                    __Print(this, copyBuffer);
                    break;
                case 'p':
                    ConvertHexadecimalToString(copyBuffer, va_arg(list, uint64), true);
                    __Print(this, copyBuffer);
                    break;
                /*case 'f':
                    copyBuffer = ConvertNumberToString('f', va_arg(list, double));
                    __Print(this, copyBuffer);
                    break;*/
                default:
                    __Print(this, string + index);
            }
        } else {
            switch (string[index]) {
                case '\n':
                    ++this->cursor.y;
                    this->cursor.x = 0;
                    break;
                default:
                    WriteAscii(this->screen, 8 * this->cursor.x, 16 * this->cursor.y, string[index], this->foregroundColor);
                    ++this->cursor.x;
            }
        }
    }

    va_end(list);

    this->screen->Refresh(this->screen);
}

static code WriteAscii(
  const SCREEN*           screen,
  size                    x,
  size                    y,
  const byte              character,
  const struct PixelColor color) {
    size        dx, dy;
    const byte* font = fonts + 16ull * (addr)character;

    for (dy = 0; dy < 16; ++dy) {
        for (dx = 0; dx < 8; ++dx) {
            if ((font[dy] << dx) & 0x80u) {
                screen->WriteBuffer(screen, x + dx, y + dy, color);
            }
        }
    }

    return SUCCESS;
}
