#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include <unistd.h>
#include <omp.h>

#include "omp_producer_consumer.h"

int main(int argc, char* argv[]) {
  int   prod_count, cons_count;
  FILE* files[MAX_FILES];
  int   file_count;

  if (argc != 3) Usage(argv[0]);
  prod_count = strtol(argv[1], NULL, 10);
  cons_count = strtol(argv[2], NULL, 10);

  /* Read in list of filenames and open files */
  Get_files(files, &file_count);

#  ifdef DEBUG
  printf("prod_count = %d, cons_count = %d, file_count = %d\n",
         prod_count, cons_count, file_count);
#  endif

  /* Producer-consumer */
  Prod_cons(prod_count, cons_count, files, file_count);

  return 0;
}  /* main */


/*--------------------------------------------------------------------
 * Function:    Usage
 * Purpose:     Print command line for function and terminate
 * In arg:      prog_name
 */
void Usage(char* prog_name) {

  fprintf(stderr, "usage: %s <producer count> <consumer count>\n",
          prog_name);
  exit(0);
}  /* Usage */


/*--------------------------------------------------------------------
 * Function:    Get_files
 * Purpose:     Read list of files and open them
 * Out args:    files: array of files
 *              file_count_p: total number of files
 */
void Get_files(FILE* files[], int* file_count_p) {
  int i = 0;
  char filename[MAX_CHAR];

  while (scanf("%s", filename) != -1) {
    files[i] = fopen(filename, "r");
    printf("file %d = %s, desc = %p\n", i, filename, files[i]);
    if (files[i] == NULL) {
      fprintf(stderr, "Can't open %s\n", filename);
      fprintf(stderr, "Quitting . . . \n");
      exit(-1);
    }
    i++;
  }
  *file_count_p = i;
}  /* Get_files */


/*--------------------------------------------------------------------
 * Function:   Prod_cons
 * Purpose:    Divides tasks among threads
 * In args:    thread_count, files, file_count
 */
void Prod_cons(int prod_count, int cons_count, FILE* files[],
               int file_count) {
  int thread_count = prod_count + cons_count;
  struct list_node_s* queue_head = NULL;
  struct list_node_s* queue_tail = NULL;
  int prod_done_count = 0;

#  pragma omp parallel num_threads(thread_count) default(none)  \
  shared(file_count, queue_head, queue_tail, files, prod_count, \
         cons_count, prod_done_count)
  {
    int my_rank = omp_get_thread_num(), f;
    if (my_rank < prod_count) {
      /* Producer code */
      /* A cyclic partition of the files among the producers */
      for (f = my_rank; f < file_count; f += prod_count) {
#           ifdef DEBUG
        printf("Th %d > about to read file %d\n", my_rank, f);
#           endif
        Read_file(files[f], &queue_head, &queue_tail, my_rank);
#           ifdef DEBUG
//          Print_queue(my_rank, queue_head);
#           endif
      }
#        pragma omp atomic
      prod_done_count++;
    }
  }
#  pragma omp parallel num_threads(thread_count) default(none)  \
  shared(file_count, queue_head, queue_tail, files, prod_count, \
         cons_count, prod_done_count)
    {
      int my_rank = omp_get_thread_num();
      if (my_rank >= prod_count) {
        /* Consumer code */
        struct list_node_s* tmp_node;

        while (prod_done_count < prod_count) {
          tmp_node = Dequeue(&queue_head, &queue_tail, my_rank);
          if (tmp_node != NULL) {
            Tokenize(tmp_node->data, my_rank);
            free(tmp_node);
          }
        }
#        ifdef DEBUG
        Print_queue(my_rank, queue_head);
#        endif
        while (queue_head != NULL) {
          tmp_node = Dequeue(&queue_head, &queue_tail, my_rank);
          if (tmp_node != NULL) {
            Tokenize(tmp_node->data, my_rank);
            free(tmp_node);
          }
        }
      }
    }
}  /* Prod_cons */

/*--------------------------------------------------------------------
 * Function:    Read_file
 * Purpose:     read text line from file into the queue linkedlist
 * In arg:      file, my_rank
 * In/out arg:  queue_head, queue_tail
 */

void Read_file(FILE* file, struct list_node_s** queue_head,
               struct list_node_s** queue_tail, int my_rank) {

  char* line = malloc(MAX_CHAR*sizeof(char));
  while (fgets(line, MAX_CHAR, file) != NULL) {
    printf("Th %d > read line: %s", my_rank, line);
    Enqueue(line, queue_head, queue_tail);
    line = malloc(MAX_CHAR*sizeof(char));
  }

  fclose(file);
}  /* Read_file */


/*--------------------------------------------------------------------
 * Function:    Enqueue
 * Purpose:     create data node, add into queue linkedlist
 * In arg:      line
 * In/out arg:  queue_head, queue_tail
 */
void Enqueue(char* line, struct list_node_s** queue_head,
             struct list_node_s** queue_tail) {
  struct list_node_s* tmp_node = NULL;

  tmp_node = malloc(sizeof(struct list_node_s));
  tmp_node->data = line;
  tmp_node->next = NULL;

#  pragma omp critical
  if (*queue_tail == NULL) { // list is empty
    *queue_head = tmp_node;
    *queue_tail = tmp_node;
  } else {
    (*queue_tail)->next = tmp_node;
    *queue_tail = tmp_node;
  }
}  /* Enqueue */


/*--------------------------------------------------------------------
 * Function:    Dequeue
 * Purpose:     remove a node from queue linkedlist and tokenize them
 * In arg:      my_rank
 * In/out arg:  queue_head, queue_tail
 * Ret val:     Node at head of queue, or NULL if queue is empty
 */
struct list_node_s* Dequeue(struct list_node_s** queue_head,
                            struct list_node_s** queue_tail, int my_rank) {
  struct list_node_s* tmp_node = NULL;

  if (*queue_head == NULL){
    return NULL;
  } // empty

#  pragma omp critical
  {
    if (*queue_head == *queue_tail) // last node
      *queue_tail = (*queue_tail)->next;

    tmp_node = *queue_head;
    *queue_head = (*queue_head)->next;
  }

  return tmp_node;
}  /* Dequeue */

/*--------------------------------------------------------------------
 * Function:    Tokenize
 * Purpose:     Identify words (i.e., sequences of characters separated
 *                 by white space and print them out)
 * In arg:      data, my_rank
 *
 */

void Tokenize(char* data, int my_rank) {
  char* my_token, *word;

#  ifdef DEBUG
  printf("Th %d > dequeue line: %s\n", my_rank, data);
#  endif
  my_token = strtok_r(data, " \t\n", &word);
  while (my_token != NULL) {
    printf("Th %d token: %s\n", my_rank, my_token);
    my_token = strtok_r(NULL, " \t\n", &word);
  }
}  /* Tokenize */

/*--------------------------------------------------------------------
 * Function:    Print_queue
 * Purpose:     Print the contents of the queue
 * In args:     queue_head, queue_tail
 */
void Print_queue(int my_rank, struct list_node_s* queue_head) {
  struct list_node_s* curr_p = queue_head;

  printf("Th %d > queue = \n", my_rank);
#  pragma omp critical
  while (curr_p != NULL) {
    printf("%s", curr_p->data);
    curr_p = curr_p->next;
  }
  printf("\n");
}
