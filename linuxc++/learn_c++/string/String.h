#include <iostream>
#include <cstring>
class String
{
    public:
        String(const char* str = "")
            :_str(new char[(strlen(str) + 5)]) // 多开辟4个字节来存放引用计数
            {
                *((int*)_str) = 1;
                _str += 4; // str中内容在引用计数后
                strcpy(_str, str);

            }

        String(const String& s)
        {
            _str = s._str; // 基本与浅拷贝相似
            ++GetRefCount();

        }

        String& operator=(const String& s)
        {
            if (_str != s._str)
            {
                if (--(GetRefCount()) == 0)
                {
                    delete[] _str;

                }

                _str = s._str;
                ++(GetRefCount());


            }

            return *this;

        }

        ~String()
        {
            if (--(GetRefCount()) == 0)
            {
                delete[] (_str - 4);

            }

        }

        int& GetRefCount() // 返回引用类型，也可写成指针类型
        {
            return *((int*)(_str - 4));

        }
    private:
        char* _str;

};
