#ifndef PLOYMORPHISM_H__
#define PLOYMORPHISM_H__
#include "String.h"
#include <ostream>

class Class{
    protected:
        String Num;
        int students;
    public:
        Class(String,int);
        String getclass(){return this->Num;}
        Class& operator=(const Class&);

};

class Person{
    protected:
        String name;
        String num;
    public:
        Person(){}
        void set_val(String,String);
        Person& operator=(const Person&);
        std::ostream operator<<(const Person&);

};
class student:public Person{
    String myclass;
    public:
    student(){}
    int addclass(Class&);
    void show();
};

class teacher:public Person{
    public:
        friend class Class;

};

#endif
