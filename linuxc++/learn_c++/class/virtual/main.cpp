#include <polymorphism.h>
int main(){
    String str("1838");
    Class class3(str,30);

    String name("yixingwei");
    String num("6578");

    Person *p1;
    student stu = student();
    p1 = &stu;
    p1->set_val(name,num);
    stu.addclass(class3);

    stu.show();

    exit(0);
}
