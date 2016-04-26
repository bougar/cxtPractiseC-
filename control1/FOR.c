#include <stdint.h>

void foo(uint64_t a) {
  uint64_t v = 0;
  for ( a = 111 ; a == 222 ; a = a + 999 ) {
    v = 1;
  }
}

