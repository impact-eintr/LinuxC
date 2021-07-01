#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* struct for queue */
struct list_node_s {
  char* data;
  struct list_node_s* next;
};

void Usage(char* prog_name);
void Prod_cons(int prod_count, int cons_count, FILE* files[], int file_count); void Get_files(FILE* files[], int* file_count_p);
void Read_file(FILE* file, struct list_node_s** queue_head, struct list_node_s** queue_tail, int my_rank);
void Enqueue(char* line, struct list_node_s** queue_head, struct list_node_s** queue_tail);
struct list_node_s* Dequeue(struct list_node_s** queue_head, struct list_node_s** queue_tail, int my_rank);
void Tokenize(char* data, int my_rank);
void Print_queue(int my_rank, struct list_node_s* queue_head);

int main(int argc, char** argv)
{

}
