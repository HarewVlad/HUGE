#pragma once

#include "Arena.h"

struct Node {
  Node *next;
  Node *prev;
  void *data;
};

struct List {
  Node *node = nullptr;
  int size = 0;
};

List GetList();
void ListPush(List *list, void *data);
void ListPop(List *list);
void ListPopAt(List *list, int index);
void ListFree(List *list);