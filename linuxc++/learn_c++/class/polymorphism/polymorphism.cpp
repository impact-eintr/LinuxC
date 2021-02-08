#include "polymorphism.h"

Class::Class(String Num,int students){
    this->Num = Num;
    this->students = students;
}

Class& Class::operator=(const Class &newclass){
    Class *temp = nullptr;
    temp->Num =  newclass.Num;
    temp->students = newclass.students;
    return *temp;
}


void Person::set_val(String name,String num){
    this->name = name;
    this->num = num;
}
/*
   std::ostream Person::operator<<(const Person& p){
   printf("%s",p.name._str);
   return p.num;

   }
   */

void student::show(){
    printf("%s\n",this->name._str);
    printf("%s\n",this->num._str);
    printf("%s\n",this->myclass._str);

}
int student::addclass(Class &newclass){
    String temp = newclass.getclass();
    this->myclass = temp;
    return 0;
}

int main(){
    String str("1839");
    Class class3(str,30);

    String name("yixingwei");
    String num("6578");

    student stu = student();
    Person *p1 = &stu;
    stu.set_val(name,num);
    stu.addclass(class3);

    //std::cout<<stu<<std::endl;
    stu.show();


    exit(0);

}
