#include "Main.h"

void Main(const struct FRAMEBUFFER_CONFIG* frameBufferConfig) {
    uint32_t x, y;

    for (x = 0; x < (*frameBufferConfig).horizontalResolution; ++x) {
        for (y = 0; y < (*frameBufferConfig).verticalResolution; ++y) {
            WritePixel(frameBufferConfig, x, y, (struct PixelColor) { 0, 0, 0 });
        }
    }

    WriteString(frameBufferConfig, 0, 0, "Hello from DummyOS!", (struct PixelColor) { 255, 255, 255 });
    
    Halt();
}