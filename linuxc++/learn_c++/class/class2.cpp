#include <iostream>
#include <cmath>
class Square{
    int *width,*height;
    public:
    static int n;
    Square();
    Square(int,int);
    ~Square();
    int area(void){return (*width * *height);}
};
class Circle{
    int radius;
    public:
    Circle(){}
    Circle(int);
    void set_val(int);
    double area(void){return (radius * M_PI);
    };
};

int Square::n = 6;

Square::Square(){
    *(this->width) = 5;
    *(this->height) = 5;

}

Square::Square(int x,int y){
    width = new int;
    height = new int;
    *width = x;
    *height = y;
}

Square::~Square(){
    delete width;
    delete height;
}

Circle::Circle(int x){
    this->radius = x;
}

void Circle::set_val(int x){
    this->radius = x;
}

int main(){

    Circle *circles = new Circle[2];
    circles[0].set_val(4);
    double ret  = circles[0].area();
    printf("%f\n",ret);


}
