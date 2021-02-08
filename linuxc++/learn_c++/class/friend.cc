#include "String.h"

class Class{
    String num;
    int students;
    public:
    Class(String,int);
    String getclass(){return this->num;}

};

class Person{
    protected:
        String name;
        String num;
    public:
        void set_val(String,String);
        Class operator=(const Class&);

};

class student:public Person{
    public:
        friend class Class;
        int addclass(class Class);
};

class teacher:public Person{
    public:
        teacher(String,String);
        friend class Class;

};

Class::Class(String num,int students){
    this->num = num;
    this->students = students;
}

void Person::set_val(String name,String num){
    this->name = name;
    this->num = num;
}

int student::addclass(const Class myclass){
    this->Class = myclass;
}

int mian(){
    String str("1839");
    Class class3(str,30);


    exit(0);

}
