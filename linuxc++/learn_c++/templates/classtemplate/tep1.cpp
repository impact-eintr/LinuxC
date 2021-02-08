#include <bits/stdint-intn.h>
#include <iostream>

class Integer{
    public:
        int num;
        Integer(int num){this->num = num;}
        int operator>(Integer b){
            if(this->num > b.num)
              return 1;
            return 0;
        }
        int operator<(Integer b){
            if(this->num < b.num)
              return 1;
            return 0;
        }
#if 0
        int operator<<(Integer a){
            if(this == &a)
              return this->num;
            return 0;
        }
#endif
};
template <typename T> class compare {
    T value[2];

    public:
    compare(T a, T b) {
        value[0] = a;
        value[1] = b;
    }

    T Max() { return (value[0] > value[1]) ? value[0] : value[1]; }
    T Min() { return (value[0] < value[1]) ? value[0] : value[1]; }
};


int main() {
    compare<int> cp1(2,3);
    int max = cp1.Max();

    Integer a(1);
    Integer b(4);
    compare<Integer*> cp2(&a,&b);
    Integer *ptr = cp2.Min();
    int min = ptr->num;
    //std::cout << max  <<"\n"<< min->num<<std::endl;
    std::cout <<*(min).num;
    std::cout << std::endl;

}
