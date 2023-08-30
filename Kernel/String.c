#include "String.h"

size GetStringLength(
  const byte* string) {
    size length = 0;

    for (length = 0; string[length] != '\0'; ++length);

    return length;
}

void CopyString(
  const byte* source,
  byte* destination) {
    size length = GetStringLength(source);

    destination[length] = '\0';

    while (--length > 0) {
        destination[length] = source[length];
    }

    destination[0] = source[0];
}

void ConvertDecimalToString(byte* string, uint64 number, bool sign) {
    uint64 index = 0;
    uint64 length = 0;

    byte buffer[100] = {0, };

    if (number == 0) {
        string[0] = '0';
        string[1] = '\0';

        return ;
    }

    if (sign and number > MAX_INT64) {
        buffer[0] = '-';
        number = -(int64)number;
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
        while (index > 0) {
            string[length - index] = buffer[index];
            --index;
        }
        string[length] = buffer[0];
    }

    string[length + 1] = '\0';
}

//byte* FloatToString(byte* string, double number) {
//    
//}

void ConvertHexadecimalToString(byte* string, uint64 number, bool caps) {
    uint64 index = 0;
    uint64 length = 0;

    byte buffer[100] = { 0, };

    if (number == 0) {
        string[0] = '0';
        string[1] = '\0';

        return;
    }

    while (number > 0) {
        if (caps) {
            buffer[index] = (number % 16 >= 10) ? 'A' + (number % 16 - 10) : '0' + number % 16;
        } else {
            buffer[index] = (number % 16 >= 10) ? 'a' + (number % 16 - 10) : '0' + number % 16;
        }
        number /= 16;
        ++index;
    }

    --index;
    length = index;

    while (index > 0) {
        string[length - index] = buffer[index];
        --index;
    }

    string[length] = buffer[0];
    string[length + 1] = '\0';
}