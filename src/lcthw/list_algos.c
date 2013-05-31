#include "list_algos.h"
#include "dbg.h"


inline void ListNode_swap(ListNode *a, ListNode *b)
{
  check(a->value != NULL && b->value != NULL, "Can't swap NULL values.");
  void *temp = a->value;
  a->value = b->value;
  b->value = temp;

error:
  return;
}

int List_bubble_sort(List *list, List_compare cmp)
{
  // int sorted = 1;

  if(List_count(list) <= 1) {
    return 0; // already sorted
  }

   
  int count = List_count(list);

  do {
    int current = 0;
    int reducer = 0; 
    LIST_FOREACH_LIMIT(list, first, next, cur, count, reducer) {
      if(cur->next) {
        if(cmp(cur->value, cur->next->value) > 0) {
          ListNode_swap(cur, cur->next);
          reducer = current;
        }
      }
      current++;
    }
    count = reducer;
  } while(count != 0);
   
   return 0;
}

inline List *List_merge(List *left, List *right, List_compare cmp)
{
  List *result = List_create();
  void *val = NULL;

  while(List_count(left) > 0 || List_count(right) > 0) {
    if(List_count(left) > 0 && List_count(right) > 0) {
      if(cmp(List_first(left), List_first(right)) <= 0) {
        val = List_shift(left);
      } else {
        val = List_shift(right);
      }

      List_push(result, val);
    } else if(List_count(left) > 0) {
      val = List_shift(left);
      List_push(result, val);
    } else if(List_count(right) > 0) {
      val = List_shift(right);
      List_push(result, val);
    }
  }

  return result;

}


List *List_merge_sort(List *list, List_compare cmp)
{
  if(List_count(list) <= 1) {
    return list; // already sorted
  }

  List *left = List_create();
  List *right = List_create();

  int middle = List_count(list) / 2;

  LIST_FOREACH(list, first, next, cur) {
    if(middle > 0) {
      List_push(left, cur->value);
    } else {
      List_push(right, cur->value);
    }

    middle--;
  }

  List *sort_left = List_merge_sort(left, cmp);
  List *sort_right = List_merge_sort(right, cmp);

  if(sort_left != left) { List_destroy(left); }
  if(sort_right != right) { List_destroy(right); }

  return List_merge(sort_left, sort_right, cmp);
}


List *List_insert_sorted(List *list, void *value, List_compare cmp) {

  check(value != NULL, "Can't insert NULL to list.")

  if(list->count == 0) {
    List_push(list, value);
  } else if(list->count == 1) {
    if(cmp(List_first(list), value) >= 0) {
      List_unshift(list, value);
    } else {
      List_push(list, value);
    }
  } else {
    List_push(list, value);
    list = List_merge_sort(list, cmp);
  }

  return list;

error:
  return NULL;
}