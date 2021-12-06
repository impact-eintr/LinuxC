#ifndef STRING_H_
#define STRING_H_

#include <string.h>
#include <ostream>

class String
{
public:
  String(const char* cstr = 0);
  String(const String& str);
  String& operator=(const String& str);
  ~String();

  char* get_c_str() const { return m_data; };
private:
  char *m_data;
};

std::ostream& operator<<(std::ostream& os, const String& str);

#endif // STRING_H_
