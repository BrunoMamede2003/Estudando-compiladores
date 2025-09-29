#include <list.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <macros.h>
#include <stdbool.h>

struct __list {
  void* m_data;
  size_t m_element_size;
  size_t m_size;
  size_t m_capacity;
  void (*m_element_destructor)(void*);
};

List * const list_new(
  size_t element_size,
  size_t capacity,
  void (*element_destructor)(void*))
{
  List * const obj = malloc(sizeof(List));
  if(not obj) {
    perror("Memory error in list_new");
    return NULL;
  }
  *obj = (List){NULL, element_size, 0, capacity, element_destructor};
  obj->m_data = malloc(element_size * capacity);
  if(not obj->m_data) {
    perror("Memory error in list_new");
    free(obj);
    return NULL;
  }
  return obj;
}

void list_free(List * const list_ptr) {
  if(list_ptr == NULL)
    return;
  if(list_ptr->m_element_destructor)
    for(size_t index = 0; index < list_ptr->m_size; index += 1)
      list_ptr->m_element_destructor(list_access(list_ptr, index));
  free(list_ptr->m_data);
  free(list_ptr);
}

static int double_cap(List * const ptr) {
  void * new_ptr = realloc(ptr->m_data, ptr->m_element_size * ptr->m_capacity * 2);
  if(new_ptr == NULL)
    return 0;
  ptr->m_data = new_ptr;
  ptr->m_capacity *= 2;
  return 1;
}

void * const list_access(List * const list_ptr, size_t const index) {
  if(not list_ptr)
    return NULL;
  if(index >= list_ptr->m_size)
    return NULL;
  return (char*)list_ptr->m_data + (index * list_ptr->m_element_size);
}

void list_push_back(List *const list_ptr, const void * const item) {
  if((list_ptr->m_size >= list_ptr->m_capacity) and (not double_cap(list_ptr))) {
    perror("Memory error in list_push_back");
    return;
  }
  memcpy(
    list_ptr->m_data + (list_ptr->m_size * list_ptr->m_element_size),
    item,
    list_ptr->m_element_size);
  list_ptr->m_size += 1;
}

void list_insert(List *const list_ptr, const void * const item, const size_t index) {
  if(index > list_ptr->m_size) {
    perror("Insertion out of bounds in list_insert");
    return;
  }
  else if(index == list_ptr->m_size) {
    list_push_back(list_ptr, item);
  }
  if((list_ptr->m_size >= list_ptr->m_capacity) and (not double_cap(list_ptr))) {
    perror("Memory error in list_insert");
    return;
  }
  memmove(
    list_ptr->m_data + ((index + 1) * list_ptr->m_element_size),
    list_ptr->m_data + (index * list_ptr->m_element_size),
    (list_ptr->m_size - index) * list_ptr->m_element_size);
  memcpy(
    list_ptr->m_data + (index * list_ptr->m_element_size),
    item,
    list_ptr->m_element_size);
  list_ptr->m_size += 1;
}

void list_remove(List * const list_ptr, size_t const index) {
  if(index == list_ptr->m_size - 1 or list_ptr->m_size == 1) {
    list_ptr->m_size -= 1;
    return;
  }
  memmove(
    list_access(list_ptr, index),
    list_access(list_ptr, index + 1),
    (list_ptr->m_size - index - 1) * list_ptr->m_element_size 
  );
  list_ptr->m_size -= 1;
}

size_t list_size(const List * const list_ptr) {
  return list_ptr->m_size;
}

void list_for_each(List * const list_ptr, void (*function)(void * const)) {
  for(size_t i = 0; i < list_ptr->m_size; i++)
    function(list_access(list_ptr, i));
}

List * const list_filter(List * const list_ptr, int (*function)(void const * const)) {
  List * const l = list_new(list_ptr->m_element_size, list_ptr->m_capacity, list_ptr->m_element_destructor);
  if(not l) {
    perror("Memory error ocurred when calling list_new from list_filter");
    return NULL;
  }
  for(size_t i = 0; i < list_ptr->m_size; i++)
    if(function(list_access(list_ptr, i)))
      list_push_back(l, list_access(list_ptr, i));
  return l;
}

void * const list_as_array(List * const list_ptr) {
  return list_ptr->m_data;
}

List * const list_copy(List * const list_ptr) {
  List * nl = malloc(sizeof(List));
  if(not nl) {
    perror("Memory error in list_copy");
    return NULL;
  }
  *nl = *list_ptr;
  nl->m_data = malloc(nl->m_element_size * nl->m_capacity);
  if(not nl->m_data) {
    perror("Memory error in list_copy");
    free(nl);
    return NULL;
  }
  memcpy(nl->m_data, list_ptr->m_data, nl->m_element_size * nl->m_size);
  return nl;
}

Slice const null_slice = {NULL, 0, 0};

Slice slice_from_list(List * const lptr, size_t start_index, size_t max_size) {
  if(start_index >= lptr->m_size)
    return null_slice;
  return (Slice){lptr, start_index, max_size + start_index <= lptr->m_size ? max_size : lptr->m_size - start_index};
}

Slice slice_from_slice(Slice s, size_t start_index, size_t max_size) {
  const size_t true_start = start_index + s.m_start;
  if(true_start >= s.m_list->m_size)
    return null_slice;
  return (Slice){s.m_list, true_start, max_size + true_start <= s.m_list->m_size ? max_size : s.m_list->m_size - true_start};
}

void slice_push_back(Slice * const s, void const * const item) {
  list_insert(s->m_list, item, s->m_start + s->m_size);
  s->m_size += 1;
}

void slice_insert(Slice * const s, void const * const item, size_t index) {
  if(index > s->m_size) {
    perror("Attempt to insert beyond slice size in slice_insert");
    return;
  }
  list_insert(s->m_list, item, s->m_start + index);
  s->m_size += 1;
}
void slice_remove(Slice * const s, size_t index) {
  if(index > s->m_size) {
    perror("Attempt to remove beyond slice size in slice_remove");
    return;
  }
  list_remove(s->m_list, s->m_start + index);
  s->m_size -= 1;
}

void * slice_access(Slice const s, size_t index) {
  return list_access(s.m_list, s.m_start + index);
}

size_t slice_size(Slice s) {
  return s.m_size;
}

List * const slice_generate_list(Slice s) {
  const size_t nsize = s.m_size;
  List * const obj = malloc(sizeof(List));
  if(not obj) {
    perror("Memory error in slice_generate_list");
    return NULL;
  }
  *obj = (List){
    malloc(s.m_list->m_element_size * nsize),
    s.m_list->m_element_size,
    nsize,
    nsize,
    s.m_list->m_element_destructor
  };
  if(not (obj->m_data)) {
    perror("Memory error in slice_generate_list");
    free(obj);
    return NULL;
  }
  memcpy(
    obj->m_data,
    s.m_list->m_data + (s.m_list->m_element_size * s.m_start),
    nsize * s.m_list->m_element_size);
  return obj;
}