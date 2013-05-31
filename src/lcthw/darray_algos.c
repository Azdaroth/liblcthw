#include "darray_algos.h"
#include <stdlib.h>

void swap_values(void **a, void **b)
{
  void *temp;
  temp = *a;
  *a = *b;
  *b = temp;
}

int qusort(void **content, int first, int last, int count, int size, DArray_compare cmp)
{

  check(count > 0, "Count can't be 0.");

  int i = first;
  int j = last;

  char *pivot = content[(first + last)/2];

  check(pivot != NULL, "Pivot Can't be NULL.");

  while (i <= j) {
    
    while(cmp(&content[i], &pivot) < 0) i++;
    while(cmp(&pivot, &content[j]) < 0) j--;

    if (i <= j) {
      swap_values(&content[i], &content[j]);
      i++;
      j--;
    }
  }

  if (first <  j) qusort(content, first, j, count, size, cmp);
  if (i < last) qusort(content, i, last, count, size, cmp);

  return 0;

error:
  return -1;
}


void merge(void **content, int first, int middle, int last, int size, DArray_compare cmp)
{

  DArray *arr = DArray_create(size, last - first);  

  int i = first;
  int j = middle + 1;

  // push to the helper array sorted values
  // iterates through the values and chooses smaller
  while (i <= middle && j <= last) {
    if (cmp(&content[j], &content[i]) < 0) {
      DArray_push(arr, content[j]);
      j++;
    } else {
      DArray_push(arr, content[i]);
      i++;
    }
  } 

  // if something is left, push it (odd numbers case)
  if (i <= middle) {
    while (i <= middle ) {
      DArray_push(arr, content[i]);
      i++;
    }
  } else {
    while (j <= last) {
      DArray_push(arr, content[j]);
      j++;
    }
  }

  for(i = 0; i <= last - first; i++) {
    content[first + i] = DArray_get(arr, i);
  }
}


int lcthw_mergesort(void **content, int first, int last, int count, int size, DArray_compare cmp)
{
  
  // can't be run forever
  // difference between first and last (what is passed as these arguments)
  // gets smaller with each iteration
  if (first != last) {  
    int middle =  (first + last)/2;
    lcthw_mergesort(content, first, middle, count, size, cmp);
    lcthw_mergesort(content, middle + 1, last, count, size, cmp);
    // divide left and right table of contents each time to the minimum size (1)
    merge(content, first, middle, last, size, cmp);
    // merge it in the end   
  }

  return 0;
}

int check_heap(void **content, int count, DArray_compare cmp)
{
  int i;
  int result = 0;

  for (i = 1; i <= count-1; i++) {
    if (cmp(content[i],  content[i/2]) > 0) {
      result = 1;
      break;
    }
  }

  return result;
}

void shift_down(void **content, int start, int end, DArray_compare cmp)
{

  int child, swap;
  int root = start;

  while ((root*2 + 1) <= end) {
    child = root*2 + 1;
    swap = root;


    if (cmp(&content[swap], &content[child]) < 0) swap = child;
    printf("SHIFTING DOWN\n");
    if ((child + 1) <= end && cmp(&content[swap], &content[child+1]) < 0) swap = child +1;
    if (swap != root) {
      swap_values(&content[root], &content[swap]);
    } else {
      break;
    }
  }
}

void build_heap(void **content, int count, DArray_compare cmp)
{
  int start = (count-2)/2;

  while (start >= 0) {
    shift_down(content, start, count-1, cmp);
    start--;
  }
}


int lcthw_heapsort(void **content, int count, DArray_compare cmp)
{

  check(count >= 1, "There's nothing to sort");

  int end = count - 1;
  while (end > 0) {
    swap_values(&content[end], &content[0]);
    end--;
    shift_down(content, 0, end, cmp);
  }

  return 0;

error:
  return -1;
}


int DArray_qusort(DArray *array, DArray_compare cmp)
{
  qusort(array->contents, 0, DArray_count(array) - 1, DArray_count(array), sizeof(void *), cmp);
  return 0;
}

int DArray_lcthw_mergesort(DArray *array, DArray_compare cmp)
{
  return lcthw_mergesort(array->contents, 0, DArray_count(array) -1, DArray_count(array), sizeof(void *), cmp);
}

int DArray_lcthw_heapsort(DArray *array, DArray_compare cmp)
{
  return lcthw_heapsort(array->contents, DArray_count(array), cmp);
}


int DArray_sort(DArray *array, DArray_sort_type type, DArray_compare cmp)
{
  return type(array->contents, 0, DArray_count(array) - 1, DArray_count(array), sizeof(void *), cmp);
}

int DArray_add_sort(DArray *array, void *el, DArray_sort_type type, DArray_compare cmp)
{
  DArray_push(array, el);
  DArray_sort(array, type, cmp);

  return 0;
}

int DArray_find(DArray *array, void *to_find, DArray_compare cmp)
{
  int low = 0;
  int high = DArray_count(array)-1;
  void **data = array->contents;

  while(low <= high) {
    int middle = low + (high - low)/2;
    if (cmp(&to_find, &data[middle]) < 0) {
      high = middle - 1;
    } else if (cmp(&to_find, &data[middle]) > 0)  {
      low = middle + 1;
    } else {
      return middle;
    }
  }

  return -1;

}
