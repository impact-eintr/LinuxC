#ifndef COMMON_H_
#define COMMON_H_

#include <stdint.h>

#define DEBUG_PRINTCACHEST (0x8)
#define DEBUG_CACHEDETAILS (0x10)
#define DEBUG_MMU (0x20)
#define DEBUG_LOADER (0x80)
#define DEBUG_PARSEINST (0x100)

#define DEBUG_VERBOSE_SET (0x241)

// do page walk
#define DEBUG_ENABLE_PAGE_WALK (0)

// type converter
uint32_t uint2float(uint32_t u);

typedef enum {
  STRING2UINT_LEADING_SPACE,
  STRING2UINT_FIRST_ZERO,
  STRING2UINT_POSITIVE_DEC,
  STRING2UINT_POSITIVE_HEX,
  STRING2UINT_NEGATIVE,
  STRING2UINT_NEGATIVE_FIRST_ZERO,
  STRING2UINT_NEGATIVE_DEC,
  STRING2UINT_NEGATIVE_HEX,
  STRING2UINT_ENDING_SPACE,
  STRING2UINT_FAILED,
} string2uint_state_t;
string2uint_state_t string2uint_next(string2uint_state_t state, char c, uint64_t *bmap);
uint64_t string2uint(const char* str);
uint64_t string2uint_range(const char* str, int start, int end);

#define MAX_INSTRUCTION_CHAR (64)

#endif // COMMON_H_
