#include<iostream>
using   namespace   std;

int main()
{
    char   str1[30], str2[30], str3[30];
    cout << "请输入你的姓名：";
    cin >> str1;
    cout << "请输入你的住址：";
    cin.ignore();
    cin.getline(str2, 30);
    cout << "请输入你的籍贯：";
    cin.ignore();
    cin.getline(str3, 30);
    cout << str3;

}
