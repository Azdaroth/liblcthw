#include "dbg.h"
#include "bstree_hash.h"
#include <stdlib.h>
#include "bstrlib.h"
#include <stdint.h>

static int default_compare(void *a, void *b)
{
  return bstrcmp((bstring)a, (bstring)b);
}

uint32_t hash_cmp(uint32_t a, uint32_t b)
{
  return  a - b;
}

uint32_t default_hash(void *data)
{
  bstring s = (bstring)data;
  uint32_t a = 1, b = 0;
  int i = 0;

  for(i = 0; i < blength(s); i++) {
    a = (a + bchare(s, i, 0)) % 65521;
    b = (b + a) % 65521;
  }

  return (b << 16) | a;
}



BSTree *BSTree_create(BSTree_compare compare)
{
  BSTree *map = calloc(1, sizeof(BSTree));
  check_mem(map);

  map->compare = compare == NULL ? default_compare : compare;

  return map;

error:
  if (map) {
    BSTree_destroy(map);
  }
  return NULL;
}

static int BSTree_destroy_cb(BSTreeNode *node)
{
  free(node);
  return 0;
}

void BSTree_destroy(BSTree *map)
{
  if (map) {
    BSTree_traverse(map, BSTree_destroy_cb);
    free(map);
  }
}


static inline BSTreeNode *BSTreeNode_create(BSTreeNode *parent, void *key, void *data, uint32_t hash)
{
  BSTreeNode *node = calloc(1, sizeof(BSTreeNode));
  check_mem(node);

  node->key = key;
  node->data = data;
  node->parent = parent;
  node->hash = hash;
  return node;

error:
  return NULL;
}


static inline void BSTree_setnode(BSTree *map, BSTreeNode *node, void *key, void *data, uint32_t hash)
{
  int cmp =  hash_cmp(node->hash, hash);

  if (cmp <= 0) {
    if (node->left) {
      BSTree_setnode(map, node->left, key, data, hash);
    } else {
      node->left = BSTreeNode_create(node, key, data, hash);
    }
  } else {
    if (node->right) {
      BSTree_setnode(map, node->right, key, data, hash);
    } else {
      node->right = BSTreeNode_create(node, key, data, hash);
    }
  }
}


int BSTree_set(BSTree *map, void *key, void *data)
{

  uint32_t hash = default_hash(key);

  if(map->root == NULL) {
    // first so just make it and get out
    map->root = BSTreeNode_create(NULL, key, data, hash);
    check_mem(map->root);
  } else {
    BSTree_setnode(map, map->root, key, data, hash);
  }

  return 0;
error:
  return -1;
}

static inline BSTreeNode *BSTree_getnode(BSTree *map, BSTreeNode *node, uint32_t hash)
{

  int cmp =  hash_cmp(node->hash, hash);

  if (cmp == 0) {
    return node;
  } else if(cmp < 0) {
    if(node->left) {
      return BSTree_getnode(map, node->left, hash);
    } else {
      return NULL;
    }
  } else {
    if (node->right) {
      return BSTree_getnode(map, node->right, hash);
    } else {
      return NULL;
    }
  }
}

void *BSTree_get(BSTree *map, void *key)
{
  uint32_t hash = default_hash(key);

  if (map->root == NULL) {
    return NULL;
  } else {
    BSTreeNode *node = BSTree_getnode(map, map->root, hash);
    return node == NULL ? NULL : node->data;
  }
}


static inline int BSTree_traverse_nodes(BSTreeNode *node, BSTree_traverse_cb traverse_cb)
{
  int rc = 0;

  if (node->left) {
    rc = BSTree_traverse_nodes(node->left, traverse_cb);
    if (rc != 0) return rc;
  }

  if (node->right) {
    rc = BSTree_traverse_nodes(node->right, traverse_cb);
    if (rc != 0) return rc;
  }

  return traverse_cb(node);
}

int BSTree_traverse(BSTree *map, BSTree_traverse_cb traverse_cb)
{
  if (map->root) {
    return BSTree_traverse_nodes(map->root, traverse_cb);
  }

  return 0;
}


static inline int BSTree_traverse_reverse_nodes(BSTreeNode *node, BSTree_traverse_cb traverse_cb)
{
  int rc = 0;


  if (node->right) {
      rc = BSTree_traverse_nodes(node->right, traverse_cb);
      if (rc != 0) return rc;
    }

  if (node->left) {
    rc = BSTree_traverse_nodes(node->left, traverse_cb);
    if (rc != 0) return rc;
  }

  return traverse_cb(node);
}

int BSTree_traverse_reverse(BSTree *map, BSTree_traverse_cb traverse_cb)
{
  if (map->root) {
    return BSTree_traverse_reverse_nodes(map->root, traverse_cb);
  }

  return 0;
}


static inline BSTreeNode *BSTree_find_min(BSTreeNode *node)
{

  while(node->left)  {
    node = node->left;
  }

  // recursive version
  // if (node->left) {
  //   node = BSTree_find_min(node->left);
  // }

  check(node != NULL, "NULL can't be min value.");

  return node;

error:
  return NULL;
}

static inline void BSTree_replace_node_in_parent(BSTree *map, BSTreeNode *node, BSTreeNode *new_value)
{

  if(node->parent) {
    if (node == node->parent->left) {
      node->parent->left = new_value;
    } else {
      node->parent->right = new_value;
    }
  } else {
    // this is the root so gotta change it
    map->root = new_value;
  }

  if (new_value) {
    new_value->parent = node->parent;
  }

}

static inline void BSTree_swap(BSTreeNode *a, BSTreeNode *b)
{
  void *temp = NULL;
  temp = b->key; b->key = a->key; a->key = temp;
  temp = b->data; b->data = a->data; a->data = temp;
  uint32_t temp_hash = 0;
  temp_hash = b->hash; b->hash = a->hash; a->hash = temp_hash;
}

static inline BSTreeNode *BSTree_node_delete(BSTree  *map, BSTreeNode *node, uint32_t hash)
{
  int cmp = hash_cmp(node->hash, hash);

  if (cmp < 0) {
    if (node->left) {
      return BSTree_node_delete(map, node->left, hash);
    } else {
      // not found
      return NULL;
    }
  } else if (cmp >0 ) {
    if (node->right) {
      return BSTree_node_delete(map, node->right, hash);
    } else {
      // not found
      return NULL;
    }
  } else {
    if (node->left && node->right) {
      // swap this node for the smallet node that is bigger than us
      BSTreeNode *successor = BSTree_find_min(node->right);
      BSTree_swap(successor, node);

      // this leaves the old successor with possibly a right child
      // so replace it with right child
      BSTree_replace_node_in_parent(map, successor, successor->right);

      // finally it's swapped, so return successor instead of node
      return successor;
    } else if (node->left) {
      BSTree_replace_node_in_parent(map, node, node->left);
    } else if(node->right) {
      BSTree_replace_node_in_parent(map, node, node->right);
    } else {
      BSTree_replace_node_in_parent(map, node, NULL);
    }

    return node;
  }
}

void *BSTree_delete(BSTree *map, void *key)
{
  void *data = NULL;
  uint32_t hash = default_hash(key);

  if (map->root) {
    BSTreeNode *node = BSTree_node_delete(map, map->root, hash);

    if (node) {
      data = node->data;
      free(node);
    }
  }

  return data;
}
