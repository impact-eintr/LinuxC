// function
extern void f1();
extern void f2() {}

void f3();
//void f3() {}
void f4() {}

__attribute__((weak)) extern void f5();
__attribute__((weak)) extern void f6() {}


__attribute__((weak)) void f7();
__attribute__((weak)) void f8() {}

void function() {

  f1();
  f2();
  f3();
  f4();
  f5();
  f6();
  f7();
  f8();
}

// object
//extern int d1;
//extern int d2 = 0; // 警告：‘d2’已初始化，却又被声明为‘extern’
//extern int d3 = 1; // 警告：‘d3’已初始化，却又被声明为‘extern’
//
//static int d4;
//static int d5 = 0;
//static int d6 = 1;
//
//int d7;
//int d8 = 0;
//int d9 = 1;
//
//__attribute__((weak)) int da;
//__attribute__((weak)) int db = 0;
//__attribute__((weak)) int dc = 1;
//
//void object() {
//  d1 = 2;
//  d2 = 2;
//  d3 = 2;
//  d4 = 2;
//  d5 = 2;
//  d6 = 2;
//  d7 = 2;
//  d8 = 2;
//  d9 = 2;
//  da = 2;
//  db = 2;
//  dc = 2;
//}
