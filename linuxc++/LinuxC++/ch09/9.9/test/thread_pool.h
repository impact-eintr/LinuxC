#ifndef __THREAD_POOL_H
#define __THREAD_POOL_H

#include <vector>
#include <string>
#include <pthread.h>

using namespace std;

/*执行任务的类：设置任务数据并执行*/
class CTask {
protected:
	string m_strTaskName;   //任务的名称
	void* m_ptrData;    //要执行的任务的具体数据

public:
	CTask() = default;
	CTask(string &taskName)
		: m_strTaskName(taskName)
		, m_ptrData(NULL) {}
	virtual int Run() = 0;
	void setData(void* data);   //设置任务数据
  
	virtual ~CTask() {}
    
};

/*线程池管理类*/
class CThreadPool {
private:
	static vector<CTask*> m_vecTaskList;    //任务列表
	static bool shutdown;   //线程退出标志
	int m_iThreadNum;   //线程池中启动的线程数
	pthread_t *pthread_id;
  
	static pthread_mutex_t m_pthreadMutex;  //线程同步锁
	static pthread_cond_t m_pthreadCond;    //线程同步条件变量
  
protected:
	static void* ThreadFunc(void *threadData);  //新线程的线程回调函数
	static int MoveToIdle(pthread_t tid);   //线程执行结束后，把自己放入空闲线程中
	static int MoveToBusy(pthread_t tid);   //移入到忙碌线程中去
	int Create();   //创建线程池中的线程
  
public:
	CThreadPool(int threadNum);
	int AddTask(CTask *task);   //把任务添加到任务队列中
	int StopAll();  //使线程池中的所有线程退出
	int getTaskSize();  //获取当前任务队列中的任务数
};

#endif