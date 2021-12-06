#include "String.h"
#include <ostream>
#include <cstring>

String::String(const char* cstr) {
  if (cstr) {
    m_data = new char[strlen(cstr)+1];
    strcpy(m_data, cstr);
  } else { // 未指定初值
    m_data = new char[1];
    *m_data = '\0';
  }
}

String::String(const String& str) {
  m_data = new char[strlen(str.m_data) + 1]; // 直接取另一个object的private(兄弟之间互为友元)
  strcpy(m_data, str.m_data);
}

String& String::operator=(const String& str) {
  if (this == &str) {
    return *this;
  }

  delete[] m_data;
  m_data = new char[strlen(str.m_data)+1];
  strcpy(m_data, str.m_data);
  return *this;
}

String::~String() {
  delete[] m_data;
}

std::ostream& operator<<(std::ostream& os, const String& str) {
  return os << str.get_c_str();
}
