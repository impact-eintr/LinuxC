#include "complex.h"
#include <ostream>

inline complex &complex::operator+=(const complex &r) {
  return _doapl(this, r);
}
