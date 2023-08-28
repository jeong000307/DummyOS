#!/usr/bin/python3

import argparse
import collections
import functools
import re
import sys


BITMAP_PATTERN = re.compile(r'([.*@]+)')


def compile(src: str) -> bytes:
    src = src.lstrip()
    result = "const byte* fonts = \n\t{"
    i = 0

    for line in src.splitlines():
        m = BITMAP_PATTERN.match(line)
        if not m:
            continue
        i += 1
        
        bits = [(0 if x == '.' else 1) for x in m.group(1)]
        bits_int = functools.reduce(lambda a, b: 2*a + b, bits)
        result += ("0x%0.2X" % bits_int) + ", "

        if(i % 16 == 0):
            result += "\n\t"

    return result[:-4] + "};"


def main():
    with open("Fonts.c", 'w') as out, open("Fonts.txt") as font:
        src = font.read()
        out.write(compile(src))

if __name__ == '__main__':
    main()
