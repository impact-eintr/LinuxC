#pragma once
#include <string.h>
#include <iostream>
#include <omp.h>

using namespace std;

struct Node
{
    string info;
    Node* next = nullptr;
};


struct Queue
{
    Node* head, *tail;
    int Enqueue, Dequeue;
    omp_lock_t lock;
    Queue()
    {
        head = tail = nullptr;
        Enqueue = Dequeue = 0;
        omp_init_lock(&lock);
    }
};

void Split(string info);
void push(Queue* q, string s);
string pop(Queue* q);
bool empty(Queue* q);
int length(Queue* q);
bool Done(ifstream files[]);
