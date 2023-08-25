#include "String.h"

size GetStringLength(
    const byte* string) {
    size length = 0;

    while (*string) {
        ++length;
        ++string;
    }

    return length;
}

void CopyString(byte* destination, const byte* source) {
    size length = GetStringLength(source);
    size index;

    for(index = 0; index < length; ++index) {
        destination[index] = source[index];
    }

    destination[length] = '\0';
}

void ConvertDecimalToString(byte* string, int64 number) {
    int64 index = 0;
    int64 length = 0;

    byte buffer[100] = {0, };

    if (number == 0) {
        string[0] = '0';
        string[1] = '\0';

        return ;
    }

    if (number < 0) {
        buffer[0] = '-';
        number = -number;
        ++index;
    }

    while (number > 0) {
        buffer[index] = '0' + number % 10;
        number /= 10;
        ++index;
    }

    --index;
    length = index;

    if (buffer[0] == '-') {
        string[0] = '-';
        while (index > 0) {
            string[length - index + 1] = buffer[index];
            --index;
        }
    } else {
        while (index >= 0) {
            string[length - index] = buffer[index];
            --index;
        }
    }

    string[length + 1] = '\0';
}

//byte* FloatToString(byte* string, double number) {
//    
//}

void ConvertHexadecimalToString(byte* string, uint64 number) {
    int64 index = 0;
    int64 length = 0;

    byte buffer[100] = { 0, };

    if (number == 0) {
        string[0] = '0';
        string[1] = '\0';

        return;
    }

    while (number > 0) {
        buffer[index] = (number % 16 >= 10)? 'A' + (number % 16 - 10): '0' + number % 16;
        number /= 16;
        ++index;
    }

    --index;
    length = index;

    while (index >= 0) {
        string[length - index] = buffer[index];
        --index;
    }

    string[length + 1] = '\0';
}