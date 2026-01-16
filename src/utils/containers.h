#pragma once
#include "../x86-64/memory/malloc.h"
#include "../x86-64/stdio/stdio.h"

struct LinkedListNode {
  struct LinkedListNode *next;
  void *element;
};

int linked_list_append(struct LinkedListNode *start_node, const void *element,
                       uint32_t size_of_elemnt);

int get_element_at(struct LinkedListNode *start_node, int idx, void **out);

void DESTRUCT_linked_list(struct LinkedListNode *start_node);

int test_linked_list(int x);
