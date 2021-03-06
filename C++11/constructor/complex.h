#ifndef COMOLEX_H__
#define COMOLEX_H__

class complex   //class head
{               //class body                
    public:
    complex(double r = 0,double i = 0) :re(r),im(i){}
    complex& operator += (const complex&);  //声明
    double real() const {return re;}        //定义
    double imag() const {return im;}
    ~complex() {}

    private:
    double re,im;//数据

    //friend complex* __doapl {complex*,const complex&};
};


#endif

