#ifndef TREE_H
#define TREE_H

#include <list.h>
#include <stdbool.h>

typedef struct __tree_node Tree_node;

Tree_node * const tree_new(void const * const item, size_t item_size, void (*item_destructor)(void * const));
void tree_free(Tree_node * const ptr);
void * const tree_access_item(Tree_node * const ptr);
Tree_node * tree_access_child(Tree_node const * const ptr, size_t child_index);
Tree_node * tree_access_right_brother(Tree_node const * const ptr);
Tree_node * tree_access_left_brother(Tree_node const * const ptr);
Tree_node * tree_access_father(Tree_node const * const ptr);
size_t tree_num_children(Tree_node const * const ptr);
size_t tree_num_children_recursive(Tree_node const * const ptr);

void tree_emplace_child(
  Tree_node * const ptr,
  void const * const item,
  size_t item_size,
  void (*item_destructor)(void * const));

void tree_emplace_child_at(
  Tree_node * const ptr,
  void const * const item,
  size_t item_size,
  void (*item_destructor)(void * const),
  size_t at);

void tree_unlink(Tree_node * const ptr);
void tree_link_nodes(Tree_node * const father, Tree_node * const son);
void tree_print(Tree_node * const root, void (*print_item)(void * const, size_t depth));

#endif //TREE_H