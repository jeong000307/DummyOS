#include "Console.h"

extern const byte fonts[];

code CreateSystemConsole(
  OUT SYSTEM_CONSOLE* this,
  IN  SCREEN* screen) {
    this->rows = 25;
    this->columns = 80;
    this->cursor = (struct Cursor){0, 0};

    SetMemory(this->buffer, 0, this->rows * (this->columns + 1));

    this->foregroundColor = (struct PixelColor){255, 255, 255};
    this->backgroundColor = (struct PixelColor){0, 0, 0};
    this->screen = screen;

    this->SystemPrint = __SystemPrint;

    return SUCCESS;
}

static code __SystemPrint(
  IN SYSTEM_CONSOLE* this,
  IN const byte*     string,
  IN ...) {
    size    index;
    va_list list;

    byte    copyBuffer[1000];

    va_start(list, string);

    for (index = 0; string[index] != '\0'; ++index) {
        SetMemory(copyBuffer, '\0', 1000 * sizeof(byte));

        if (this->cursor.x >= this->columns) {
            this->cursor.x = 0;
            ++this->cursor.y;
        }

        if (this->cursor.y >= this->rows) {
            CopyMemory(this->screen->frameBuffer, this->screen->GetPixelAddress(this->screen, 0, 16), this->screen->horizontalResolution * (this->screen->verticalResolution - 16) * sizeof(byte) * 3);
            --(this->cursor.y);
        }

        if (string[index] == '%') {
            ++index;

            switch (string[index]) {
                case 's':
                    CopyString(copyBuffer, va_arg(list, byte*));
                    __SystemPrint(this, copyBuffer);
                    break;
                case 'c':
                    copyBuffer[0] = va_arg(list, byte);
                    copyBuffer[1] = '\0';
                    __SystemPrint(this, copyBuffer);
                    break;
                case 'd':
                case 'i':
                    ConvertDecimalToString(copyBuffer, va_arg(list, int64));
                    __SystemPrint(this, copyBuffer);
                    break;
                case 'x':
                case 'X':
                    ConvertHexadecimalToString(copyBuffer, va_arg(list, uint32) & 0xFFFFFFFF);
                    __SystemPrint(this, copyBuffer);
                    break;
                case 'q':
                case 'Q':
                case 'p':
                    ConvertHexadecimalToString(copyBuffer, va_arg(list, uint64));
                    __SystemPrint(this, copyBuffer);
                    break;
                /*case 'f':
                    copyBuffer = ConvertNumberToString('f', va_arg(list, double));
                    __SystemPrint(this, copyBuffer);
                    break;*/
                default:
                    __SystemPrint(this, string + index);
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

    return SUCCESS;
}

code WriteAscii(
  IN OUT const SCREEN*           screen,
  IN     size                    x,
  IN     size                    y,
  IN     const byte              character,
  IN     const struct PixelColor color) {
    size        dx, dy;
    const byte* font = fonts + 16 * (uint32)character;

    for (dy = 0; dy < 16; ++dy) {
        for (dx = 0; dx < 8; ++dx) {
            if ((font[dy] << dx) & 0x80u) {
                screen->WritePixel(
                  screen, 
                  x + dx, 
                  y + dy, 
                  color);
            }
        }
    }

    return SUCCESS;
}
