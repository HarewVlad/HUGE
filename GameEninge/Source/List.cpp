#include "List.h"
#include "Utils.h"

MakeInternal List GetList() {
  List list;

  return list;
}

MakeInternal void ListPush(List *list, void *data) {
  if (list->node == nullptr) {
    list->node = (Node *)calloc(1, sizeof(Node));
    list->node->data = data;
  }
  else {
    Node *base = list->node;
    while (base->next != nullptr) {
      base = base->next;
    }
    Node *node = (Node *)calloc(1, sizeof(Node));
    node->data = data;
    node->prev = base;

    base->next = node;
  }
  list->size++;
}

MakeInternal void ListPop(List *list) {
  Node *node_to_delete = list->node;
  while (node_to_delete->next != nullptr) {
    node_to_delete = node_to_delete->next;
  }

  Node *node_before_delete = node_to_delete->prev;

  free(node_to_delete);

  if (node_before_delete != nullptr) {
    node_before_delete->next = nullptr;
  }
  else {
    list->node = nullptr;
  }

  list->size--;
}

MakeInternal void ListPopAt(List *list, int index) {
  ASSERT(list->size > index);

  Node *node_to_delete = list->node;
  
  if (index > 0) {
    for (int i = 0; i < index; ++i) {
      node_to_delete = node_to_delete->next;
    }
  } else {
    list->node = list->node->next;
  }

  if (node_to_delete->prev != nullptr) {
    node_to_delete->prev->next = node_to_delete->next;
    if (node_to_delete->next != nullptr) {
      node_to_delete->next->prev = node_to_delete->prev;
    }
  }

  free(node_to_delete);

  list->size--;
}

MakeInternal void ListFree(List *list) {
  Node *base = list->node;
  if (base != nullptr) {
    while (base->next != nullptr) {
      Node *node_to_delete = base;
      base = base->next;
      free(node_to_delete);
      base->prev = nullptr;
    }
    free(base);
    list->node = nullptr;
    list->size = 0;
  }
}