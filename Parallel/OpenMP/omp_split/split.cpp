#include <iostream>
#include <algorithm>
#include <ctime>
#include <string>
#include <fstream>

#include "split.h"

using namespace std;

void Split(string info) 
{
    cout << info + '\n';
    string word;
    for (char ch : info) {
        if (ch != ' ')
          word += ch;
        else {
            cout << word + '\n';
            word.clear();
        }
    }
    cout << word + '\n';
}

void push(Queue* q, string s) 
{
    Node* n = new Node;
    n->info = s;
    if (!q->tail) {
        q->head = n;
        q->tail = n;
    }
    else {
        Node* old_tail = q->tail;
        q->tail = n;
        old_tail->next = q->tail;
    }
    q->Enqueue++;
}

string pop(Queue* q) 
{
    if (!q->head)
      return "";
    else {
        Node* newHead = q->head->next;
        string msg = q->head->info;
        delete q->head;
        if (q->tail == q->head)
          q->tail = newHead;
        q->head = newHead;
        q->Dequeue++;
        return msg;
    }
}

bool empty(Queue* q)
{
    return (q->head == nullptr);
}

int length(Queue* q) 
{
    Node* n = q->head;
    int len = 0;
    while (n)
    {
        n = n->next;
        len++;
    }
    return len;
}


bool Done(ifstream files[])
{
    for (int i = 0;i < 4;i++) {
        if (files[i].eof())
          continue;
        else
          return false;
    }
    return true;
}

