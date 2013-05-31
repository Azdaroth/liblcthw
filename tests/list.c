#include "list.h"
#include "dbg.h"
#include <assert.h>
#include <string.h>
#include "bstrlib.h"
#include <ctype.h>

List *List_create()
{
  return calloc(1, sizeof(List));
}


// free all the nodes and the list
void List_destroy(List *list)
{
  LIST_FOREACH(list, first, next, cur) {
    if(cur->prev) {
      free(cur->prev);
    }
  }

  free(list->last);
  free(list);
}


// free all the nodes, but leave the list
void List_clear(List *list)
{
  LIST_FOREACH(list, first, next, cur) {
    free(cur->value);
  }
}


void List_clear_destroy(List *list)
{

  LIST_FOREACH(list, first, next, cur) {
    if(cur->prev) {
      free(cur->prev);
    }
  }

  free(list->last);
  free(list);

  // List_clear(list);
  // List_destroy(list);
}


// put element at the enf od the list
void List_push(List *list, void *value)
{
  ListNode *node = calloc(1, sizeof(ListNode));
  check_mem(node);

  node->value = value;

  // if list is clear
  if(list->last == NULL) {
    list->first = node;
    list->last = node;
  } else {
    list->last->next = node;
    node->prev = list->last;
    list->last = node;
  }

  list->count++;

  check(list->first != NULL, "Impossible: first element can't be NULL");
  check(list->last != NULL, "Impossible: last element can't be NULL");

  check(list->count >= 0, "Count can't be less than 0.");

error:
  return;
}


// get last element and remove it
void *List_pop(List *list) 
{
  ListNode *node = list->last;

  if (list->count > 0) {  
    check(list->first != NULL, "Impossible: first element can't be NULL");
    check(list->last != NULL, "Impossible: last element can't be NULL");
   }

  check(list->count >= 0, "Count can't be less tan 0.");

  return node != NULL ? List_remove(list, node) : NULL; 

error:
  return NULL;
}


// put the element at the beginning of the list
void List_unshift(List *list, void *value)
{
  ListNode *node = calloc(1, sizeof(ListNode));
  check_mem(node);

  node->value = value;

  if(list->first == NULL) {
    list->first = node;
    list->last = node;
  } else {
    node->next = list->first;
    list->first->prev = node;
    list->first = node;
  }

  assert(list->first != NULL);
  list->count++;

  if (list->count > 0) {  
    check(list->first != NULL, "Impossible: first element can't be NULL");
    check(list->last != NULL, "Impossible: last element can't be NULL");
   } 

   check(list->count >= 0, "Count can't be less tan 0.");

error:
  return;
}


// get the first element and remove it
void *List_shift(List *list)
{
  ListNode *node = list->first;

  if (list->count > 0) {  
    check(list->first != NULL, "Impossible: first element can't be NULL");
    check(list->last != NULL, "Impossible: last element can't be NULL");
   }

  check(list->count >= 0, "Count can't be less tan 0.");

  return node != NULL ? List_remove(list, node) : NULL;

error:
  return NULL;
}

void *List_remove(List *list, ListNode *node)
{
  void *result = NULL;

  check(list->first && list->last, "List is empty.");
  check(node, "node can't be NULL.");

  if(node == list->first && node == list->last) {
    list->first = NULL;
    list->last = NULL;
  } else if(node == list->first) {
    list->first = node->next;
    check(list->first != NULL, "Invalid list, somehow got a first that is NULL.");
    list->first->prev = NULL;
  } else if(node == list->last) {
    list->last = node->prev;
    check(list->last != NULL, "Invalid list, somehow got a net that is NULL.");
    list->last->next = NULL;
  } else {
    ListNode *after = node->next;
    ListNode *before = node->prev;
    after->prev = before;
    before->next = after;
  }

  list->count--;
  result = node->value;
  free(node);

  if (list->count > 0) {  
    check(list->first != NULL, "Impossible: first elemennt can't be NULL");
    check(list->last != NULL, "Impossible: last elemennt can't be NULL");
   }

  check(list->count >= 0, "Count can't be less tan 0.");

error:
  return result;
}


void *List_copy(List *copy_from, List *copy_to)
{
  check(copy_from->count > 0, "Can't copy list with 0 elements");

  LIST_FOREACH(copy_from, first, next, cur) {
    if(cur->value) {
      List_push(copy_to, cur->value);
    }
  }

  check(copy_to->count >= 0, "Copy failed.");

error:
  return NULL;
}

bstring List_join(List *list)
{
  check(list->count > 0, "Can't join list with 0 elements");
  
  bstring joined_str = NULL;

  LIST_FOREACH(list, first, next, cur) {
    if(cur->value) {
      if(joined_str == NULL) {
        joined_str = bfromcstr(cur->value);
      } else {
        bconcat(joined_str, bfromcstr(" "));
        bconcat(joined_str, bfromcstr(cur->value));
      } 
    }
  }

  check(joined_str != NULL, "Joining gone wrong.")

  return joined_str;

error: 
  return NULL; 
}

List *split_to_list(char string[], int size)
{

  check(size > 0, "Can't join nothing");
  List *list = List_create();

  int i = 0;
  int z = 0;
  int j = 0;

  while(j <= size-1) {
    char temp[20];
    
    for(i = j, z = 0; (!isspace(string[i]) || string[i] == '\0') && i <= size -1; i++) {
      temp[z] = string[i];
      printf("%c-%d-%d\n", temp[z], i, j);  
      z++;
      j++;
    }
    temp[z] = '\0';
    printf("AFTER FOR-%c-%d\n", temp[0], j);
    List_push(list, &temp);
    list = list;
    // printf("PUSHED!\n");
    j++;
    // printf("VALUE first!:%s\n", list->first->value);
    // printf("VALUE last!:%s\n", list->last->value);
  } 

  // printf("VALUE AT THE END!:%s count %d\n", list->first->value, list->count);

  check(list->first->value != NULL, "Error assigning values.");

  return list;

error: 
  return NULL;  
}



