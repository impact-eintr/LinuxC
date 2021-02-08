#include <iostream>

template <class T> T GetMax(T a,T b){
    return (a > b) ? a : b;
}

int main(){
    int i = 9,j = 20,k;
    long a = 8,b = 2,c;
    k = GetMax(i,j);
    c = GetMax(a,b);
    std::cout << k<<"\n"<<c<<std::endl;

}
