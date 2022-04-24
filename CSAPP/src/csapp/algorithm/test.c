#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../headers/algorithm.h"
#include "../headers/cpu.h"
#include "../headers/instruction.h"

#define TIMES (1000)

static trie_node_t *register_mapping = NULL;
static trie_node_t *operator_mapping = NULL;

void mov_handler(od_t *src_od, od_t *dst_od) {}

void push_handler(od_t *src_od, od_t *dst_od) {}

void pop_handler(od_t *src_od, od_t *dst_od) {}

void leave_handler(od_t *src_od, od_t *dst_od) {}

void call_handler(od_t *src_od, od_t *dst_od) {}

void ret_handler(od_t *src_od, od_t *dst_od) {}

void add_handler(od_t *src_od, od_t *dst_od) {}

void sub_handler(od_t *src_od, od_t *dst_od) {}

void cmp_handler(od_t *src_od, od_t *dst_od) {}

void jne_handler(od_t *src_od, od_t *dst_od) {}

void jmp_handler(od_t *src_od, od_t *dst_od) {}

void lea_handler(od_t *src_od, od_t *dst_od) {}

void int_handler(od_t *src_od, od_t *dst_od) {}

static void lazy_initialize_trie() {
  if (operator_mapping == NULL) {
    operator_mapping = trie_construct();
    operator_mapping =
        trie_insert(operator_mapping, "movq", (uint64_t)&mov_handler);
    operator_mapping =
        trie_insert(operator_mapping, "mov", (uint64_t)&mov_handler);
    operator_mapping =
        trie_insert(operator_mapping, "push", (uint64_t)&push_handler);
    operator_mapping =
        trie_insert(operator_mapping, "pushq", (uint64_t)&push_handler);
    operator_mapping =
        trie_insert(operator_mapping, "pop", (uint64_t)&pop_handler);
    operator_mapping =
        trie_insert(operator_mapping, "leaveq", (uint64_t)&leave_handler);
    operator_mapping =
        trie_insert(operator_mapping, "callq", (uint64_t)&call_handler);
    operator_mapping =
        trie_insert(operator_mapping, "retq", (uint64_t)&ret_handler);
    operator_mapping =
        trie_insert(operator_mapping, "add", (uint64_t)&add_handler);
    operator_mapping =
        trie_insert(operator_mapping, "sub", (uint64_t)&sub_handler);
    operator_mapping =
        trie_insert(operator_mapping, "cmpq", (uint64_t)&cmp_handler);
    operator_mapping =
        trie_insert(operator_mapping, "jne", (uint64_t)&jne_handler);
    operator_mapping =
        trie_insert(operator_mapping, "jmp", (uint64_t)&jmp_handler);
    operator_mapping =
        trie_insert(operator_mapping, "lea", (uint64_t)&lea_handler);
    operator_mapping =
        trie_insert(operator_mapping, "int", (uint64_t)&int_handler);
  }
}

int main() {
  //trie_node_t *tn = trie_construct();
  //trie_insert(tn, "abcdef", 2022);
  //trie_insert(tn, "aabbcc", 2);
  //trie_insert(tn, "aababc", 3);
  //uint64_t val = 0;
  //trie_get(tn, "aabbcc", &val);
  //printf("%ld\n", val);
  //val = 0;
  //trie_get(tn, "aabbdd", &val);
  //printf("%ld\n", val);
  //val = 0;
  //trie_get(tn, "aababc", &val);
  //printf("%ld\n", val);

  //trie_print(tn);
  //trie_free(tn);
  //lazy_initialize_trie();
  //trie_print(operator_mapping);

  linkedlist_t *list = linkedlist_construct();
  for (int i = 0;i < TIMES; ++i) {
    linkedlist_add(list, i);
  }

  for (int i = 300;i < TIMES/2; ++i) {
    linkedlist_node_t *head = linkedlist_index(list, i);
    linkedlist_delete(list, head);
  }

  linkedlist_free(list);
}
