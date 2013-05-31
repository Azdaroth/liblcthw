#ifndef _lcthw_Stack_darray_h
#define _lcthw_Stack_darray_h

#include "darray.h"

#define Stack DArray

#define Stack_create() DArray_create(sizeof(int), 100)
#define Stack_destroy(A) DArray_destroy(A)

#define Stack_push(A, B) DArray_push(A, B)
#define Stack_peek(A) DArray_last(A)
#define Stack_pop(A) DArray_pop(A)

#define Stack_count(A) DArray_count(A)

#define STACK_FOREACH(A, B) int index = DArray_count(A) - 1;\
  void *B = NULL;\
  for(B = A->contents[index]; index >= 0; B = A->contents[index-1], index--)

#endif
