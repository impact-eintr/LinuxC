#include <iostream>
#include <fstream>

#include "split.h"

using namespace std;

static omp_lock_t lock, lock2;

/*
 * 问题：实现并发访问和处理的队列，队列接受字符串。生
 * 产者4个线程，消费者4个线程，生产者从文件中读取文本行
 * 并把它插入目标序号的队列。同时，消费者从队列重复地读取
 * message，并用split函数分词，在分词的过程中不断输出它，
 * 直到生产者的文件都为空且消费者队列也被处理完毕。
 *
 * 分析：
 * 每个消费者对应一个队列，不同队列可以被同时访问，但
 * 一个队列不能同时被多个生产者访问，这里需要上特殊的线程锁。
 * 生产者和生产者，消费者和消费者之间不需要同步，因为任务结束条件
 * 都是一样的all(file clear and queue empty)，只需要在8个线程的
 * 任务结束阶段设置一个barrier即可。
 * 允许队列被同时push和pop，如果用链表实现queue，则删除队首元素
 * 和在队尾增加元素并不冲突，但是同时修改队列长度n会造成冲突，所以
 * 我们用enqueue和dequeue两个参数描述队列长度，就不会造成冲突。
 *
 * 文件格式：文件两行为一组，第一行为0-3的编号，表示要发送到的目标
 * 消费者线程；第二行为一段含空格的字符串，即需要发送的消息字段。
 * */
int main()
{
    omp_init_lock(&lock);
    omp_init_lock(&lock2);

    ifstream files[4] = {
        ifstream("file0.txt"),
        ifstream("file1.txt"),
        ifstream("file2.txt"),
        ifstream("file3.txt")
    };

    Queue queues[4];
    //string msg;
    //while (getline(files[0], msg))
    //{
    //  cout << msg << endl;
    //}

#   pragma omp parallel num_threads(8)
    {
        int my_rank = omp_get_thread_num();
        if (my_rank < 4) {
            int dest;string msg;
            while (getline(files[my_rank], msg)) {
                dest = msg[0] - '0';
                getline(files[my_rank], msg);
                omp_set_lock(&(queues[dest].lock));
                push(&queues[dest], msg);
                //printf("Push line into queue %d\nNow length of queue %d is %d\n", dest, dest, length(&(queues[dest])));
                omp_unset_lock(&(queues[dest].lock));
            }
        }
        else {
            int num = my_rank - 4;
            string msg;
            while (!(Done(files) and empty(&queues[num]))) {
                if (empty(&queues[num]))
                  continue;
                else {
                    omp_set_lock(&(queues[num].lock));
                    msg = pop(&queues[num]);
                    Split(msg);
                    omp_unset_lock(&(queues[num].lock));
                    //printf("Queue %d has dealed message...\nlength is %d\n", num, length(&queues[num]));
                }
            }
        }
#       pragma omp barrier
    }
}

