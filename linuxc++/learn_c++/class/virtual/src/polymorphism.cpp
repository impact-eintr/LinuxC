#include <polymorphism.h>

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


void student::set_val(String name,String num){
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
    printf("%s\t",this->name._str);
    printf("%s\t",this->num._str);
    printf("%s\t",this->myclass._str);
    printf("\n\n");

}
int student::addclass(Class &newclass){
    String temp = newclass.getclass();
    this->myclass = temp;
    return 0;
}

