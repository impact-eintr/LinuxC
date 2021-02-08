#include <iostream>

class String {
    public:
        int _size;
        int _capacity;
        char *_str;
        String(const String &s);
        String &operator=(const String &);
};
//浅拷贝
String::String(const String &s)
        : _str(s._str), _size(s._size), _capacity(s._capacity) {

        }

String &String::operator=(const String &s) {
    if (_str != s._str) {
        delete[] _str;
        _str = s._str;
        _size = s._size;
        _capacity = s._capacity;
    }
    return *this;
}
