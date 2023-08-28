#include "Error.h"

char* Error(
  code status) {
    switch(status)  {
        case SUCCESS: return "success";
        default: return "failed";
    }
}