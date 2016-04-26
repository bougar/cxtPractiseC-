#include <stdint.h>

void foo(uint64_t a) {
  uint64_t v = 0;
  v = 1;
}


void foo2(uint64_t a) {
  uint64_t v = 0;
  for ( a = 111 ; a == 222 ; a = a + 999 ) {
    v = 1;
  }
}


void foo3(uint64_t a) {
  uint64_t v = 0;
  v = v + 1;
  v = v * 27;
}

