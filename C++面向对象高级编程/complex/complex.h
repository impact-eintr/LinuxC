#ifndef COMPLEX_H_
#define COMPLEX_H_

#include <ostream>

class complex {
public:
  complex(double r = 0, double i = 0) : re(r), im(i) {} // 构造函数
  complex &operator+=(const complex &);                 // 函数声明
  complex &operator+(const complex &);                  // 函数声明
  double real() const { return re; }
  double imag() const { return im; }

private:
  double re, im;
  friend complex &_doapl(complex *, const complex &);
};

// 以下是全局函数
inline complex &_doapl(complex *ths, const complex &r) {
  ths->re += r.re;
  ths->im += r.im;
  return *ths;
}

inline double imag(const complex &x) { return x.imag(); }

inline double real(const complex &x) { return x.real(); }

inline complex operator+(const complex &x, const complex &y) {
  return complex(real(x) + real(y), imag(x) + imag(y));
}

inline complex operator+(const complex &x, const double y) {
  return complex(real(x) + y, imag(x) + y);
}

inline complex operator+(const double x, const complex &y) {
  return complex(x + real(y), x + imag(y));
}

inline complex operator+(const complex &x) { return x; }

inline complex operator-(const complex &x) {
  return complex(-real(x), -imag(x));
}

inline bool operator==(const complex &x, const complex &y) {
  return real(x) == real(y) && imag(x) == imag(y);
}

inline bool operator==(const complex &x, const double &y) {
  return real(x) == y && imag(x) == 0;
}

inline bool operator==(const double &x, const complex &y) {
  return x == real(y) && imag(y) == 0;
}

inline bool operator!=(const complex &x, const complex &y) {
  return real(x) != real(y) || imag(x) != imag(y);
}

inline bool operator!=(const complex &x, const double &y) {
  return real(x) != y || imag(x) != 0;
}

inline bool operator!=(const double &x, const complex &y) {
  return x != real(y) || imag(y) != 0;
}

// 共轭复数
inline complex conj(const complex &x) { return complex(real(x), -imag(x)); }

inline std::ostream &operator<<(std::ostream &os, const complex &x) {
  return os << "(" << real(x) << "," << imag(x) << ")";
}

#endif // COMPLEX_H_
