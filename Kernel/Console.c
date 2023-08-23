#include "Console.h"

static const char* GetFont(
  IN char character) {
    uint32_t index = 16 * (uint32_t)character;

    return fonts + index;
}

static void WriteAscii(
  IN OUT const SCREEN*           screen,
  IN     uint32_t                x,
  IN     uint32_t                y,
  IN     char                    character,
  IN     const struct PixelColor color) {
    uint32_t    dx, dy;
    const char* font = GetFont(character);

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
}

void WriteString(
    IN OUT const SCREEN*           screen,
    IN     uint32_t                x,
    IN     uint32_t                y,
    IN     const char*             string,
    IN     const struct PixelColor color) {
    uint32_t index;

    for (index = 0; string[index] != '\0'; ++index) {
        WriteAscii(
          screen, 
          x + 8 * index, 
          y, 
          string[index], 
          color);
    }
}
