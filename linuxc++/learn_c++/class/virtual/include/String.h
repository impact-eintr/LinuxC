#ifndef STRING_H__
#define STRING_H__

#include  <iostream>
#include <cstring>

class String
{
    public:
        String(const char* str = "");
        String(const String& s);
        String& operator=(const String& s);
        ~String();
        char* _str;

};

#endif
