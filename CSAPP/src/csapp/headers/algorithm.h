#ifndef ALGORITHM_H_
#define ALGORITHM_H_

#include <stdint.h>

#define DEFARG(name, defval) ((#name[0]) ? (name + 0) : defval)
#define NULL_ID (0)

/*======================================*/
/*      Dynamic Array                   */
/*======================================*/
typedef struct {
  uint32_t size;
  uint32_t count;
  uint64_t *table;
} array_t;

array_t *array_construct(int size);
void array_free(array_t *arr);
array_t *array_insert(array_t *arr, uint64_t value);
int array_delete(array_t *arr, int index);
int array_get(array_t *arr, int index, uint64_t *valptr);

/*======================================*/
/*      Extendible Hash Table           */
/*======================================*/
typedef struct {
  int localdepth;
  int counter;
  char **karray;
  uint64_t *varray;
} hashtable_bucket_t;

typedef struct {
  int num;
  int globaldepth;

  int size;
  hashtable_bucket_t **directory;
} hashtable_t;

#define hashtable_construct(arg0) _hashtable_construct(DEFARG(arg0, 32));
hashtable_t *_hashtable_construct(int size);
void hashtable_free(hashtable_t *tab);
int hashtable_get(hashtable_t *tab, char *key, uint64_t *valptr);
hashtable_t *hashtable_insert(hashtable_t *tab, char *key, uint64_t val);

/*======================================*/
/*      Trie - Prefix Tree              */
/*======================================*/
typedef struct TRIE_NODE_STRUCT {
  hashtable_t *next;
  uint64_t value;
  int isvalue;
} trie_node_t;

trie_node_t *trie_construct();
void trie_free(trie_node_t *root);
trie_node_t *trie_insert(trie_node_t *root, char *key, uint64_t value);
int trie_get(trie_node_t *root, char *key, uint64_t *valptr);
trie_node_t *trie_next(trie_node_t *current, char input);

/*======================================*/
/*      Circular Doubly Linked List     */
/*======================================*/

/*======================================*/
/*      Red Black Tree                  */
/*======================================*/

/*======================================*/
/*      Binary Search Tree              */
/*======================================*/

#endif // ALGORITHM_H_
