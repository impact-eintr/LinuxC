#ifndef ALGORITHM_H_
#define ALGORITHM_H_

#include <stdint.h>

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

#define DEFARG(name, defval) ((#name[0]) ? (name + 0) : defval)
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
typedef struct {
  uint64_t (*construct_node)();             // alloc memory for a node
  int (*destruct_node)(uint64_t);           // deallocate memory for a node
  int (*is_null_node)(uint64_t);            // Check a node is null
  int (*compare_nodes)(uint64_t, uint64_t); // Check if two nodes are the same

  uint64_t (*get_node_prev)(uint64_t);
  int (*set_node_prev)(uint64_t, uint64_t);

  uint64_t (*get_node_next)(uint64_t);
  int (*set_node_next)(uint64_t, uint64_t);

  uint64_t (*get_node_value)(uint64_t);
  int (*set_node_value)(uint64_t, uint64_t);
} linkedlist_node_interface;

#define ILIST_CONSTRUCT (1 << 0)
#define ILIST_DESTRUCT (1 << 1)
#define ILIST_CHECKNULL (1 << 2)
#define ILIST_COMPARE (1 << 3)
#define ILIST_PREV (1 << 4)
#define ILIST_NEXT (1 << 5)
#define ILIST_VALUE (1 << 6)
void linkedlist_validate_interface(linkedlist_node_interface *i_node, uint64_t flags);

typedef struct LINKEDLIST_IMTERNAL_STRUCT {
  uint64_t head;
  int64_t count;

  int (*update_head)(struct LINKEDLIST_IMTERNAL_STRUCT *, uint64_t); // this: this pointer
} linkedlist_internal_t;

int linkedlist_internal_add(linkedlist_internal_t *list,
                            linkedlist_node_interface *inode, uint64_t value);
int linkedlist_internal_insert(linkedlist_internal_t *list,
                               linkedlist_node_interface *inode, uint64_t node);
int linkedlist_internal_delete(linkedlist_internal_t *list,
                               linkedlist_node_interface *inode, uint64_t node);
uint64_t linkedlist_internal_index(linkedlist_internal_t *list,
                                   linkedlist_node_interface *inode,
                                   uint64_t index);
uint64_t linkedlist_internal_next(linkedlist_internal_t *list,
                                  linkedlist_node_interface *inode);

/*===============================================*/
// The implementation of the default linked list
/*===============================================*/
typedef union {
  linkedlist_internal_t base;
  struct {
    uint64_t head;
    int64_t count;
  };
} linkedlist_t;

typedef struct LINKED_LIST_NODE_STRUCT {
  uint64_t value;
  struct LINKED_LIST_NODE_STRUCT *prev;
  struct LINKED_LIST_NODE_STRUCT *next;
} linkedlist_node_t;

linkedlist_t *linkedlist_construct();
void linkedlist_free(linkedlist_t *list);
void linkedlist_add(linkedlist_t *list, uint64_t value);
void linkedlist_delete(linkedlist_t *list, linkedlist_node_t *node);
linkedlist_node_t *linkedlist_index(linkedlist_t *list, uint64_t index);
linkedlist_node_t *linkedlist_next(linkedlist_t *list);

/*======================================*/
/*      Red Black Tree                  */
/*======================================*/

/*======================================*/
/*      Binary Search Tree              */
/*======================================*/

#endif // ALGORITHM_H_
