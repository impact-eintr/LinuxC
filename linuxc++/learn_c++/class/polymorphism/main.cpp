#include "polymorphism.h"
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
