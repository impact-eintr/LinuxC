#include "vgastr.h"

inline void _strwrite(char* string) {
  char *p_strdst = (char*)(0xb8000); // 指向显存的开始地址
  while(*string) {
    *p_strdst = *string++;
    p_strdst += 2;
  }
  return;
}

inline void printf(char* fmt, ...) {
  _strwrite(fmt);
  return;
}
