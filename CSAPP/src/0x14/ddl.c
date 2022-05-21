#include <stdint.h>
#include <string.h>

#define MAX_GOT_SIZE (64)

typedef void(*func_t)();
typedef uint64_t plt_t;

uint64_t got[MAX_GOT_SIZE];

uint64_t dynamic_linker(uint64_t address) {
  // calculate
  return 0xFFFFFFFFFFFFFFFF;
}

void call(uint64_t address) {
  func_t func;;
  *(uint64_t *)&func = address;
  func();
}

void plt_call(uint64_t address) {
  plt_t plt_address = address;

  if (got[plt_address] == 0) {
    // not overwritten
    got[plt_address] = dynamic_linker(address);
  } else {
    // found
    call(got[plt_address]);
  }
}

int main() {
  memset(got, 0, sizeof(uint64_t) * MAX_GOT_SIZE);
}
