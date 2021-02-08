#include <iostream>

template <class T> T GetMax(T a,T b){
    T result;
    result = (a > b) ? a : b;
    return (result);
}

int main(){
    int i = 9,j = 3,k;
    long a = 8,b = 2,c;
    k = GetMax(i,j);
    c = GetMax(a,b);
    std::cout << k<<"\n"<<c<<std::endl;

}
