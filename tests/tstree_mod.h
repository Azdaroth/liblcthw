#ifndef _lctwh_TSTree_h
#define _lctwh_TSTree_h

#include <stdlib.h>
#include "darray.h"

typedef struct TSTree {
  char splitchar;
  struct TSTree *low;
  struct TSTree *equal;
  struct TSTree *high;
  DArray *values;
} TSTree;

DArray *TSTree_search(TSTree *root, const char *key, size_t len);

DArray *TSTree_search_prefix(TSTree *root, const char *key, size_t len);

DArray *TSTree_get_all_for_prefix(TSTree *root, const char *key, size_t len);

DArray *TSTree_collect(TSTree *root, const char *key, size_t len);

DArray *TSTree_get_keys(TSTree *node, DArray *all_keys);

typedef void (TSTree_traverse_cb)(DArray *values, void *data);

TSTree *TSTree_insert(TSTree *node, const char *key, size_t len, void *value);

void TSTree_traverse(TSTree *node, TSTree_traverse_cb cb, void *data);

void TSTree_destroy(TSTree *root);

int TSTree_delete_values(TSTree *root, const char *key, size_t len);


#endif
