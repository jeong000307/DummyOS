#include "Main.h"

void Main(unsigned long long frameBufferBase, unsigned long long frameBufferSize) {
    unsigned char* frameBuffer = (unsigned char*)frameBufferBase;
    unsigned long long index;

    for (index = 0; index < frameBufferSize; ++index) {
        frameBuffer[index] = index % 256;
    }
    
    while (1) { Halt(); }
}