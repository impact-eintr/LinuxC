#include "thread_pool.h"
#include <cstdio>

void CTask::setData(void* data) {
	m_ptrData = data;
}

//静态成员初始化
vector<CTask*> CThreadPool::m_vecTaskList;
bool CThreadPool::shutdown = false;
pthread_mutex_t CThreadPool::m_pthreadMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t CThreadPool::m_pthreadCond = PTHREAD_COND_INITIALIZER;

//线程管理类构造函数
CThreadPool::CThreadPool(int threadNum) {
	this->m_iThreadNum = threadNum;
	printf("I will create %d threads.\n", threadNum);
	Create();
}

//线程回调函数
void* CThreadPool::ThreadFunc(void* threadData) {
	pthread_t tid = pthread_self();
	while (1)
	{
		pthread_mutex_lock(&m_pthreadMutex);
		//如果队列为空，等待新任务进入任务队列
		while (m_vecTaskList.size() == 0 && !shutdown)
			pthread_cond_wait(&m_pthreadCond, &m_pthreadMutex);
        
		//关闭线程
		if (shutdown)
		{
			pthread_mutex_unlock(&m_pthreadMutex);
			printf("[tid: %lu]\texit\n", pthread_self());
			pthread_exit(NULL);
		}
        
		printf("[tid: %lu]\trun: ", tid);
		vector<CTask*>::iterator iter = m_vecTaskList.begin();
		//取出一个任务并处理之
		CTask* task = *iter;
		if (iter != m_vecTaskList.end())
		{
			task = *iter;
			m_vecTaskList.erase(iter);
		}
        
		pthread_mutex_unlock(&m_pthreadMutex);
        
		task->Run();    //执行任务
		printf("[tid: %lu]\tidle\n", tid);
        
	}
    
	return (void*)0;
}

//往任务队列里添加任务并发出线程同步信号
int CThreadPool::AddTask(CTask *task) { 
	pthread_mutex_lock(&m_pthreadMutex);    
	m_vecTaskList.push_back(task);  
	pthread_mutex_unlock(&m_pthreadMutex);  
	pthread_cond_signal(&m_pthreadCond);    
    
	return 0;
}

//创建线程
int CThreadPool::Create() { 
	pthread_id = new pthread_t[m_iThreadNum];
	for (int i = 0; i < m_iThreadNum; i++)
		pthread_create(&pthread_id[i], NULL, ThreadFunc, NULL);
        
	return 0;
}

//停止所有线程
int CThreadPool::StopAll() {    
    //避免重复调用
	if (shutdown)
		return -1;
	printf("Now I will end all threads!\n\n");
    
	//唤醒所有等待进程，线程池也要销毁了
	shutdown = true;
	pthread_cond_broadcast(&m_pthreadCond);
    
	//清楚僵尸
	for (int i = 0; i < m_iThreadNum; i++)
		pthread_join(pthread_id[i], NULL);
    
	delete[] pthread_id;
	pthread_id = NULL;
    
	//销毁互斥量和条件变量
	pthread_mutex_destroy(&m_pthreadMutex);
	pthread_cond_destroy(&m_pthreadCond);
    
	return 0;
}

//获取当前队列中的任务数
int CThreadPool::getTaskSize() {    
	return m_vecTaskList.size();
}