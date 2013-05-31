#ifndef darray_algos_h
#define darray_algos_h

#include "darray.h"

typedef int (*DArray_compare)(const void *a, const void *b);
typedef int (*DArray_sort_type)(void **content, int first, int last, int count, int size, DArray_compare cmp);

int DArray_qusort(DArray *array, DArray_compare cmp);

int DArray_lcthw_mergesort(DArray *array, DArray_compare cmp);

int DArray_lcthw_heapsort(DArray *array, DArray_compare cmp);

int DArray_sort(DArray *array, DArray_sort_type type, DArray_compare cmp);

int qusort(void **content, int first, int last, int count, int size, DArray_compare cmp);

int lcthw_mergesort(void **content, int first, int last, int count, int size, DArray_compare cmp);

int DArray_add_sort(DArray *array, void *el, DArray_sort_type type, DArray_compare cmp);

int DArray_find(DArray *array, void *to_find, DArray_compare cmp);

#endif