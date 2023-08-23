#include "Main.h"

void Main(
  IN const struct FRAME_BUFFER_CONFIG* frameBufferConfig) {
    SCREEN*  screen;
    uint32_t x, y;

    CreateScreen(
      screen, 
      frameBufferConfig);

    for (x = 0; x < screen->horizontalResolution; ++x) {
        for (y = 0; y < screen->verticalResolution; ++y) {
            screen->WritePixel(
              screen, 
              x, 
              y, 
              (struct PixelColor) { 0, 0, 0 });
        }
    }

    WriteString(
      screen, 
      0, 
      0, 
      "Hello from DummyOS!", 
      (struct PixelColor) { 255, 255, 255 });
    
    Halt();
}