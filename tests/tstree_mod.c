#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "dbg.h"
#include "tstree_mod.h"

#define DEFAULT_DARRAY_SIZE 100

static inline TSTree *TSTree_insert_base(TSTree *root, TSTree *node,
          const char *key, size_t len, void *value)
{
  if(node == NULL) {
    node = (TSTree *) calloc(1, sizeof(TSTree));

    check(node != NULL, "Failed creating node");

    node->values = DArray_create(DEFAULT_DARRAY_SIZE, DEFAULT_DARRAY_SIZE);

    if(root == NULL) root = node;

    node->splitchar = *key;
  }

  if(*key < node->splitchar) {
    node->low = TSTree_insert_base(root, node->low, key, len, value);
  } else if(*key == node->splitchar) {
    if(len > 1) {
      node->equal = TSTree_insert_base(root, node->equal, key + 1, len - 1, value);
    } else {
        DArray_push(node->values, value);
        return node;
        // assert(node->value == NULL && "Duplicate insert into tst.");
    }
  } else {
      node->high = TSTree_insert_base(root, node->high, key, len, value);
  }

  return node;

error:
  return NULL;
}

TSTree *TSTree_insert(TSTree *node, const char *key, size_t len, void *value)
{

  check(key != NULL, "Key can't be NULL");
  check(value != NULL, "Value can't be NULL");
  check(len > 0, "Size can't be 0 or negative");

  return TSTree_insert_base(node, node, key, len, value);

error:
  return NULL;
}



static inline TSTree *TSTree_loop_and_get_equal(TSTree *root, const char *key, size_t len)
{

  TSTree *node = root;
  size_t i = 0;

  while(i < len && node) {
    if(key[i] < node->splitchar) {
      node = node->low;
    } else if(key[i] == node->splitchar) {
      i++;
      if(i < len) node = node->equal;
    } else {
      node = node->high;
    }
  }

  return node;

}

DArray *TSTree_search(TSTree *root, const char *key, size_t len)
{

  check(root != NULL, "Root can't be NULL");
  check(key != NULL, "Key can't be NULL");
  check(len > 0, "Size can't be 0 or negative");

  TSTree *node = TSTree_loop_and_get_equal(root, key, len);

  if(node) {
    return node->values;
  } else {
    return NULL;
  }

error:
  return NULL;
}

static inline DArray *TSTree_all_values(TSTree *node, DArray *values)
{


  if(node == NULL) return NULL;

  if(values == NULL) values = DArray_create(DEFAULT_DARRAY_SIZE, DEFAULT_DARRAY_SIZE);

  if(!DArray_empty(node->values)) DArray_push(values, DArray_first(node->values));

  if(node->low) TSTree_all_values(node->low, values);
  if(node->equal) TSTree_all_values(node->equal, values);
  if(node->high) TSTree_all_values(node->high, values);

  return values;
}

DArray *TSTree_get_all_for_prefix(TSTree *root, const char *key, size_t len)
{

  check(root != NULL, "Root can't be NULL");
  check(key != NULL, "Key can't be NULL");
  check(len > 0, "Size can't be 0 or negative");

  TSTree *node = TSTree_loop_and_get_equal(root, key, len);

  if(node == NULL) return NULL;

  DArray *values = TSTree_all_values(node, NULL);

  return values;
error:
  return NULL;
}


// DArray *TSTree_get_keys(TSTree *node, DArray *all_keys)
// {

//   if(all_keys == NULL) {
//     DArray *all_keys = DArray_create(DEFAULT_DARRAY_SIZE, DEFAULT_DARRAY_SIZE);
//   }

//   // printf("%s\n", &node->splitchar);

//    // DArray_push(all_keys, &node->splitchar);

//   if(node->low) {
//     TSTree_get_keys(node->low, all_keys);
//   }

//   if(node->equal) {
//     TSTree_get_keys(node->equal, all_keys);
//   }

//   if(node->high) {
//     TSTree_get_keys(node->high, all_keys);
//   }

//   return all_keys;
// }

// DArray *TSTree_collect(TSTree *node, const char *key, size_t len)
// {

//   DArray *all_keys = TSTree_get_keys(node, NULL);

//   DArray *match_keys = DArray_create(DEFAULT_DARRAY_SIZE, DEFAULT_DARRAY_SIZE);

//   return match_keys;
// }

DArray *TSTree_search_prefix(TSTree *root, const  char *key, size_t len)
{

  check(root != NULL, "Root can't be NULL");
  check(key != NULL, "Key can't be NULL");
  check(len > 0, "Size can't be 0 or negative");

  TSTree *node = root;
  TSTree *last = NULL;
  size_t i = 0;

  while(i < len && node) {
    if(key[i] < node->splitchar) {
      node = node->low;
    } else if(key[i] == node->splitchar) {
      i++;
      if(i < len) {
        if(!DArray_empty(node->values)) last = node;
        node = node->equal;
      }
    } else {
      node = node->high;
    }
  }

  node = node ? node : last;

  // traverse until we find the first value in the equal chain
  // this is then the first node with the prefix
  while(node && DArray_empty(node->values)) {
    node = node->equal;
  }

  return node ? node->values : NULL;

error:
  return NULL;
}

void TSTree_traverse(TSTree *node, TSTree_traverse_cb cb, void *data)
{

  check(data != NULL, "Data can't be NULL");

  if(!node) return;

  if(node->low) TSTree_traverse(node->low, cb, data);

  if(node->equal) TSTree_traverse(node->equal, cb, data);

  if(node->high) TSTree_traverse(node->high, cb, data);

  if(!DArray_empty(node->values) && node != NULL) cb(node->values, data);

error:
  return;
}


static inline void TSTree_empty_content(DArray *darray)
{

  check(darray != NULL, "DArray can't be NULL.");

   int i = 0;
    if(!DArray_empty(darray)) {
        for(i = 0; i <= DArray_count(darray); i++) {
            DArray_pop(darray);
        }
    }

error:
  return;
}

int TSTree_delete_values(TSTree *root, const char *key, size_t len)
{

  check(root != NULL, "Root can't be NULL");
  check(key != NULL, "Key can't be NULL");
  check(len > 0, "Size can't be 0 or negative");

  TSTree *node = TSTree_loop_and_get_equal(root, key, len);

  if(node) {
    TSTree_empty_content(node->values);

    return 1;
  } else {
    return 0;
  }

error:
 return -1;
}


void TSTree_destroy(TSTree *node)
{
  if(node == NULL) return;

  if(node->low) TSTree_destroy(node->low);

  if(node->equal) TSTree_destroy(node->equal);

  if(node->high) TSTree_destroy(node->high);


  //  causes core dumped, why?
  // DArray_clear_destroy(&node->values);

  free(node);
}
