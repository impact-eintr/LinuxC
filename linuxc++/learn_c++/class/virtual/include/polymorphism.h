#ifndef PLOYMORPHISM_H__
#define PLOYMORPHISM_H__
#include "String.h"

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
        virtual void set_val(String,String)=0;
        Person& operator=(const Person&);
        std::ostream operator<<(const Person&);

};
class student:public Person{
    String myclass;
    public:
    student(){}
    void set_val(String,String);
    int addclass(Class&);
    void show();
};


#endif
