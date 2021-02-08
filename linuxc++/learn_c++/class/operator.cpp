#include <iostream>

class vector2d{
    int x,y;
    public:
    vector2d(){}
    vector2d(int,int);
    void set_val(int,int);
    int get_val(int*,int*);
    vector2d operator +(const vector2d&);

};


vector2d::vector2d(int x,int y){
    this->x = x;
    this->y = y;
}

void vector2d::set_val(int x,int y){
    this->x = x;
    this->y = y;
}

int vector2d::get_val(int* x,int* y){
    *x = this->x;
    *y = this->y;
    return 0;
}
#if 0
在原来的代码里面，表示引用的符号&暗示函数返回的是一个地址，但要注意temp是一个临时变量，当函数体执行结束时，temp的内存空间也随机消失了，而我们不能返回一个不存在的地址
vector2d& vector2d::operator +(const vector2d& param){
    vector2d temp;
    temp.x = this->x+param.x;
    temp.y = this->y+param.y;
    return temp;
}
#endif

vector2d vector2d::operator +(const vector2d& param){
    vector2d temp;
    temp.x = this->x+param.x;
    temp.y = this->y+param.y;
    return temp;
}
int main(){
    vector2d *v = new vector2d[3];
    (v)->set_val(1,2);
    (v+1)->set_val(3,4);

    int x ,y =0;
    *(v+2) = *(v)+ *(v+1);
    (v+2)->get_val(&x,&y);
    printf("(%d,%d)\n",x,y);

    *(v+2) = *(v);
    (v+2)->get_val(&x,&y);
    printf("(%d,%d)\n",x,y);

}
