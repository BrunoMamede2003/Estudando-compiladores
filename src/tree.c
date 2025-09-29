#include <tree.h>
#include <list.h>
#include <macros.h>
#include <stdio.h>
#include <string.h>

struct __tree_node {
  void * m_item;
  Tree_node * m_father;
  List * m_children;
  void (*m_item_destructor)(void * const);
  size_t m_this_index;
};

Tree_node * const tree_new(void const * const item, size_t item_size, void (*item_destructor)(void * const)) {
  Tree_node * const root = malloc(sizeof(Tree_node));
  if(not root) {
    perror("Memory error in tree_new");
    return NULL; 
  }
  root->m_item = malloc(item_size);
  if(not root->m_item) {
    perror("Memory error in tree_new");
    free(root);
    return NULL;
  }
  memcpy(root->m_item, item, item_size);
  root->m_father = NULL;
  root->m_children = list_new(sizeof(Tree_node *), 2, NULL);
  if(not root->m_children) {
    perror("Error calling list_new from tree_new");
    free(root->m_item);
    free(root);
    return NULL;
  }
  root->m_item_destructor = item_destructor;
  root->m_this_index = 0;
  return root;
}

static void free_rec_aux(Tree_node * const ptr) {
  if(not ptr)
    return;
  for(size_t i = 0; i < list_size(ptr->m_children); i += 1)
    free_rec_aux(deref_as(Tree_node *, list_access(ptr->m_children, i)));
  list_free(ptr->m_children);
  ptr->m_children = NULL;
  if(ptr->m_item_destructor)
    ptr->m_item_destructor(ptr->m_item);
  else
   free(ptr->m_item);
  ptr->m_item = NULL;
  free(ptr);
}

void tree_free(Tree_node * const ptr) {
  if(not ptr)
    return;
  for(size_t i = 0; i < list_size(ptr->m_children); i += 1) {
    free_rec_aux(deref_as(Tree_node *, list_access(ptr->m_children, i)));
  }
  list_free(ptr->m_children);
  ptr->m_children = NULL;
  tree_unlink(ptr);
  if(ptr->m_item_destructor)
    ptr->m_item_destructor(ptr->m_item);
  else
    free(ptr->m_item);
  ptr->m_item = NULL;
  free(ptr);
}

void * const tree_access_item(Tree_node * const ptr) {
  return ptr->m_item;
}

Tree_node * tree_access_child(Tree_node const * const ptr, size_t child_index) {
  Tree_node ** ret = list_access(ptr->m_children, child_index);
  if(not ret)
    return NULL;
  return *ret;
}

Tree_node * tree_access_right_brother(Tree_node const * const ptr) {
  if(not ptr->m_father)
    return NULL;
  Tree_node ** ret = list_access(ptr->m_father->m_children, ptr->m_this_index + 1);
  if(not ret)
    return NULL;
  return *ret;
}

Tree_node * tree_access_left_brother(Tree_node const * const ptr) {
  if(not ptr->m_father)
    return NULL;
  Tree_node ** ret = list_access(ptr->m_father->m_children, ptr->m_this_index - 1);
  if(not ret)
    return NULL;
  return *ret;
}

Tree_node * tree_access_father(Tree_node const * const ptr) {
  return ptr->m_father;
}

size_t tree_num_children(Tree_node const * const ptr) {
  return list_size(ptr->m_children);
}

size_t tree_num_children_recursive(Tree_node const * const ptr) {
  size_t sum = 0;
  for(size_t i = 0; i < list_size(ptr->m_children); i += 1)
    sum += tree_num_children_recursive(deref_as(Tree_node *, list_access(ptr->m_children, i)));
  sum += list_size(ptr->m_children);
  return sum;
}

void tree_emplace_child(
  Tree_node * const ptr,
  void const * const item,
  size_t item_size,
  void (*item_destructor)(void * const))
{
  Tree_node * child = tree_new(item, item_size, item_destructor);
  if(not child) {
    perror("Fail in call to tree_new inside tree_emplace_child");
    return;
  }
  list_push_back(ptr->m_children, &child);
  child->m_father = ptr;
  child->m_this_index = list_size(ptr->m_children) - 1;
}

void tree_emplace_child_at(
  Tree_node * const ptr,
  void const * const item,
  size_t item_size,
  void (*item_destructor)(void * const),
  size_t at)
{
  if(at > list_size(ptr->m_children)) {
    perror("Attempt to insert child of bounds in tree_emplace_child_at");
    return;
  }
  Tree_node * child = tree_new(item, item_size, item_destructor);
  if(not child) {
    perror("Failing in call to tree_new inside tree_emplace_child");
    return;
  }
  list_insert(ptr->m_children, &child, at);
  for(size_t i = at + 1; i < list_size(ptr->m_children); i += 1)
    deref_as(Tree_node *, list_access(ptr->m_children, i))->m_this_index = i;
  child->m_father = ptr;
}

void tree_unlink(Tree_node * const ptr) {
  if(not ptr->m_father)
    return;
  list_remove(ptr->m_father->m_children, ptr->m_this_index);
  for(size_t i = ptr->m_this_index; i < list_size(ptr->m_father->m_children); i += 1)
    deref_as(Tree_node *, list_access(ptr->m_father->m_children, i))->m_this_index = i;
  ptr->m_this_index = 0;
  ptr->m_father = NULL;
}

void tree_link_nodes(Tree_node * const father, Tree_node * const son) {
  tree_unlink(son);
  list_push_back(father->m_children, &son);
  son->m_father = father;
  son->m_this_index = list_size(father->m_children) - 1;
}

static void tree_print_aux(
  Tree_node * const node,
  void (*print_item)(void * const, size_t depth),
  List * last_flags,
  size_t depth,
  bool is_last)
{
  for (size_t i = 0; i < depth; i++) {
    bool *flag = list_access(last_flags, i);
    if (*flag)
      printf("    ");
    else
      printf("│   ");
  }
  if (depth > 0) {
    printf("%s", is_last ? "└── " : "├── ");
  }
  if (print_item)
    print_item(tree_access_item(node), depth);
  else
    printf("(null item)");
  printf("\n");
  list_push_back(last_flags, &is_last);
  size_t n = tree_num_children(node);
  for (size_t i = 0; i < n; i++) {
    Tree_node * child = tree_access_child(node, i);
    tree_print_aux(child, print_item, last_flags, depth + 1, i == n - 1);
  }
  list_remove(last_flags, list_size(last_flags) - 1);
}

void tree_print(
  Tree_node * const root,
  void (*print_item)(void * const, size_t depth))
{
  if (not root)
    return;
  List * last_flags = list_new(sizeof(bool), 8, NULL);
  if (not last_flags) {
    perror("Memory error in tree_print");
    return;
  }
  tree_print_aux(root, print_item, last_flags, 0, true);
  list_free(last_flags);
}