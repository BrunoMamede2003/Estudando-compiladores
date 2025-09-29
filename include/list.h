#ifndef LIST_H
#define LIST_H

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

//OBS: don't insert itens in list that originate from the list itself, cause im using memcpy in insertion.

typedef struct __list List;

List * const list_new(
  size_t element_size,
  size_t capacity,
  void (*element_destructor)(void*));

void list_free(List * const list_ptr);
void list_push_back(List * const list_ptr, void const * const item);
void list_insert(List * const list_ptr, void const * const item, size_t const index);
void list_remove(List * const list_ptr, size_t const index);
void * const list_access(List * const list_ptr, size_t const index);
size_t list_size(List const * const list_ptr);
void list_for_each(List * const list_ptr, void (*function)(void * const));
List * const list_filter(List * const list_ptr, int (*function)(void const * const));
void * const list_as_array(List * const list_ptr);
List * const list_copy(List * const list_ptr);

struct __slice {
  List * const m_list;
  size_t m_start;
  size_t m_size;
};

typedef struct __slice Slice;

extern Slice const null_slice;

Slice slice_from_list(List * const lptr, size_t start_index, size_t max_size);
Slice slice_from_slice(Slice s, size_t start_index, size_t max_size);
void slice_push_back(Slice * const s, void const * const item);
void slice_insert(Slice * const s, void const * const item, size_t index);
void slice_remove(Slice * const s, size_t index);
void * slice_access(Slice const s, size_t index);
size_t slice_size(Slice s);
List * const slice_generate_list(Slice s);

#endif //LIST_H