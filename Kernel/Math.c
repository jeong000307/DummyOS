#include "Math.h"

uint64 GetAbsoluteValue(
  int64 value) {
    return value > 0? value: -value;
}