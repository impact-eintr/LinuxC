#include<iostream>
#include<string>

using namespace std;

int main()
{
	cout<<"find_last_of test"<<endl;
    //测试size_type find_last_of (const charT* s, size_type pos = npos) const;
    //目标串中仅有字符c与源串中的两个c匹配，其余字符均不匹配
    string str("abcdecg");
    cout << str.find_last_of("hjlywkcipn", 6) << endl;//5   从str的位置6(g)开始想前找，g不匹配，再找c，c匹配，停止查找，返回c在str中的位置5
    cout << str.find_last_of("hjlywkcipn", 4) << endl;//2   从str的位置4(e)开始想前找，e不匹配，再找d,d不匹配，再找c，c匹配，停止查找，
                                                      //    返回c在str中的位置5
    cout << str.find_last_of("hjlywkcipn", 200) << endl;//5   当第2个参数超出源串的长度（这里str长度是7）时，不会出错，相当于从源串的最后一
                                                        //    个字符起开始查找
    return 0;
}