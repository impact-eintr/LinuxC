#include <iostream>       // std::cout
#include <thread>         // std::thread, std::thread::id, std::this_thread::get_id
using namespace std;

thread::id main_thread_id =  this_thread::get_id(); //获取主线程id

void is_main_thread() 
{
	if (main_thread_id == this_thread::get_id())  //判断是否和主线程id相同
		std::cout << "This is the main thread.\n";
	else
		std::cout << "This is not the main thread.\n";
}

int main() 
{
	is_main_thread(); // is_main_thread作为main线程的普通函数调用
	thread th(is_main_thread); // is_main_thread作为线程函数使用
	th.join(); //等待th结束
	return 0;
}
