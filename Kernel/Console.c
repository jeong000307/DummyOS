#include "Console.h"
#include "AssemblyFunction.h"

extern const byte fonts[];

static SYSTEM_CONSOLE systemConsole;

SYSTEM_CONSOLE* GetSystemConsole(void) {
    return &systemConsole;
}

code InitializeSystemConsole(
  IN  SCREEN* screen) {
    systemConsole.rows = 25;
    systemConsole.columns = 80;
    systemConsole.cursor = (struct Cursor){0, 0};

    systemConsole.foregroundColor = (struct PixelColor){255, 255, 255};
    systemConsole.backgroundColor = (struct PixelColor){0, 0, 0};
    systemConsole.screen = screen;

    systemConsole.SystemPrint = __SystemPrint;

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
        if (this->cursor.x >= this->columns) {
            this->cursor.x = 0;
            ++this->cursor.y;
        }

        if (this->cursor.y >= this->rows) {
            CopyMemory(
              this->screen->GetPixelAddress(this->screen, 0, 16),
              this->screen->frameBufferConfiguration.frameBuffer,
              this->screen->GetPixelAddress(this->screen, this->screen->frameBufferConfiguration.horizontalResolution, this->screen->frameBufferConfiguration.verticalResolution) - this->screen->frameBufferConfiguration.frameBuffer);
            SetMemory(
              this->screen->GetPixelAddress(this->screen, 0, (this->rows - 1) * 16),
              0,
              this->screen->GetPixelAddress(this->screen, this->screen->frameBufferConfiguration.horizontalResolution, this->screen->frameBufferConfiguration.verticalResolution) - this->screen->GetPixelAddress(this->screen, 0, (this->rows - 1) * 16));

            this->cursor.y = this->rows - 1;
            this->cursor.x = 0;
        }

        if (string[index] == '%') {
            ++index;
            SetMemory(copyBuffer, '\0', 1000 * sizeof(byte));

            switch (string[index]) {
                case 's':
                    CopyString(va_arg(list, byte*), copyBuffer);
                    __SystemPrint(this, copyBuffer);
                    break;
                case 'c':
                    copyBuffer[0] = va_arg(list, byte);
                    copyBuffer[1] = '\0';
                    __SystemPrint(this, copyBuffer);
                    break;
                case 'd':
                case 'i':
                    ConvertDecimalToString(copyBuffer, va_arg(list, int64), true);
                    __SystemPrint(this, copyBuffer);
                    break;
                case 'u':
                    ConvertDecimalToString(copyBuffer, va_arg(list, uint64), false);
                    __SystemPrint(this, copyBuffer);
                    break;
                case 'x':
                    ConvertHexadecimalToString(copyBuffer, va_arg(list, uint64) & 0xFFFFFFFF, false);
                    __SystemPrint(this, copyBuffer);
                    break;
                case 'X':
                    ConvertHexadecimalToString(copyBuffer, va_arg(list, uint64) & 0xFFFFFFFF, true);
                    __SystemPrint(this, copyBuffer);
                    break;
                case 'p':
                    ConvertHexadecimalToString(copyBuffer, va_arg(list, uint64), true);
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

static code WriteAscii(
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
