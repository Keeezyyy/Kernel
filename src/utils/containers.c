#include "./containers.h"
#include <stdint.h>

struct LinkedListNode *get_last_node(struct LinkedListNode *start_node) {
  struct LinkedListNode *current_node = start_node;
  while (current_node->next != 0x0) {
    current_node = current_node->next;
  }

  return current_node;
}

int linked_list_append(struct LinkedListNode *start_node, const void *element,
                       uint32_t size_of_elemnt) {
  void *buffer;
  if (!k_malloc(size_of_elemnt, &buffer)) {
    printf("Error malloc for linked list element\n");
    return 0;
  }

  memcpy(buffer, element, size_of_elemnt);

  struct LinkedListNode *node_buffer;
  if (!k_malloc(sizeof(struct LinkedListNode), (void *)&node_buffer)) {
    printf("Error malloc for linked list element\n");
    return 0;
  }

  struct LinkedListNode *last_node = get_last_node(start_node);

  last_node->next = node_buffer;
  node_buffer->element = buffer;
  return 1;
}

int get_element_at(struct LinkedListNode *start_node, int idx, void **out) {
  struct LinkedListNode *current_node = start_node;

  int count = 0;
  while (current_node->next != 0x0 || idx != count) {
    current_node = current_node->next;

    count++;
  }

  if (count != idx) {
    printf("Error getting linked list element at : %p", idx);
    return 0;
  }

  *out = (void *)current_node;

  return 1;
}

void DESTRUCT_linked_list(struct LinkedListNode *start_node) {}

int test_linked_list(int y) {

  struct LinkedListNode ll_test = {.next = 0, .element = 0};

  printf("1 %p :\n", y);

  int x = 5;
  printf("2\n");
  if (!linked_list_append(&ll_test, &x, sizeof(int))) {
    printf("something went wrong linked list \n");
  }

  printf("3\n");
  return 1;
}
