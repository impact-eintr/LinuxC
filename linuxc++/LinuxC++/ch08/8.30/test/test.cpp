#include <iostream>       // std::cout
#include <thread>         // std::this_thread::sleep_until
#include <chrono>         // std::chrono::system_clock
#include <ctime>          // std::time_t, std::tm, std::localtime, std::mktime
#include <time.h>
#include <stddef.h>
using namespace std;

void getNowTime()
{
	timespec time;
	struct  tm nowTime;
	clock_gettime(CLOCK_REALTIME, &time);  //获取相对于1970到现在的秒数
	
	localtime_r(&time.tv_sec, &nowTime);
	char current[1024];
	printf(
		"%04d-%02d-%02d %02d:%02d:%02d\n",
		nowTime.tm_year + 1900,
		nowTime.tm_mon+1,
		nowTime.tm_mday, 
		nowTime.tm_hour,
		nowTime.tm_min,
		nowTime.tm_sec);
}
 
int main() 
{
	using std::chrono::system_clock;
	std::time_t tt = system_clock::to_time_t(system_clock::now());

	struct std::tm * ptm = std::localtime(&tt);
	getNowTime();
    cout << "Waiting for the next minute to begin...\n";
	++ptm->tm_min; //加一分钟
	ptm->tm_sec = 0;
	this_thread::sleep_until(system_clock::from_time_t(mktime(ptm)));
	getNowTime();
  
	return 0;
}
