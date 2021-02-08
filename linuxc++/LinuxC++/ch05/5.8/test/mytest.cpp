#include <string.h>
#include <iostream>

using namespace std;
int main(int argc, char* argv[])
{
    int i;
    cout <<"argc=" << argc << endl; //打印下传进来的参数个数

    for(i=0;i<argc;i++)   //打印各个参数
      cout<<argv[i]<<endl;

    if (argc == 2&&strcmp(argv[1], "-p")==0)  //判断是否如果带了参数-p
      cout << "will print all" << endl;
    else
      cout << "will print little" << endl;

    cout << "my program over" << endl;
    return 0;
}
