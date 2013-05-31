#include "miniunit.h"
#include "hashmap.h"
#include <assert.h>
#include "bstrlib.h"

Hashmap *map = NULL;
static int traverse_called = 0;
struct tagbstring test1 = bsStatic("test data 1");
struct tagbstring test2 = bsStatic("test data 2");
struct tagbstring test3 = bsStatic("test data 3");
struct tagbstring expect1 = bsStatic("THE VALUE 1");
struct tagbstring expect2 = bsStatic("THE VALUE 2");
struct tagbstring expect3 = bsStatic("THE VALUE 3");


static int traverse_good_cb(HashmapNode *node)
{
  debug("KEY: %s", bdata((bstring)node->key));
  traverse_called++;
  return 0;
}

static int traverse_fail_cb(HashmapNode *node)
{
  debug("KEY: %s", bdata((bstring)node->key));
  traverse_called++;

  if(traverse_called == 2) {
    return 1;
  } else {
    return 0;
  }
}

char *test_create()
{

  int size = 100;
  map = Hashmap_create(NULL, NULL, &size);
  mu_assert(map != NULL, "Failed to create map.");

  return NULL;
}

char *test_destroy()
{
  Hashmap_destroy(map);

  return NULL;
}

char *test_get_set()
{
  int rc = Hashmap_set(map, &test1, &expect1);
  mu_assert(rc == 0, "Failed to set &test1");
  bstring result = Hashmap_get(map, &test1);
  mu_assert(result == &expect1, "Wrong value for test1.");

  rc = Hashmap_set(map, &test2, &expect2);
  mu_assert(rc == 0, "Failed to set &test2");
  result = Hashmap_get(map, &test2);
  mu_assert(result == &expect2, "Wrong value for test2.");

  rc = Hashmap_set(map, &test3, &expect3);
  mu_assert(rc == 0, "Failed to set &test3");
  result = Hashmap_get(map, &test3);
  mu_assert(result == &expect3, "Wrong value for test3.");

  // // Wrong part?

  // // Set for key test3  value expect1
  // Hashmap_set(map, &test3, &expect1);
  // mu_assert(rc == 0, "Failed to set &test3");
  // // Is it supposed to always return the first value? 
  // result = Hashmap_get(map, &test3);
  // // the first value is returned, I believe the last one (case: reseting value for specified key) or array of values(case: multiple values for specified key) should be retuned
  // mu_assert(result == &expect3, "Wrong value for test3.");

  // alternative fix - reseting value

  rc = Hashmap_set(map, &test3, &expect1);
  mu_assert(rc == 0, "Failed to set &test3");
  result = Hashmap_get(map, &test3);
  mu_assert(result == &expect1, "Wrong value for reseting test3.");


  return NULL;
}

char *test_traverse()
{
  int rc = Hashmap_traverse(map, traverse_good_cb);
  mu_assert(rc == 0, "Failed to traverse.");
  // set traverse_called to 4, because of the additional wrong part
  // mu_assert(traverse_called == 4, "Wrong count traverse.");
  // alternative/original:
  mu_assert(traverse_called == 3, "Failed to traverse.");

  traverse_called = 0;
  rc = Hashmap_traverse(map, traverse_fail_cb);
  mu_assert(rc == 1, "Failed to traverse.");
  mu_assert(traverse_called == 2, "Wrong count traverse for fail.");

  return NULL;
}

char *test_delete()
{
  bstring deleted = (bstring)Hashmap_delete(map, &test1);
  mu_assert(deleted != NULL, "Got NULL on delete.");
  mu_assert(deleted == &expect1, "Should get test1");
  bstring result = Hashmap_get(map, &test1);
  mu_assert(result == NULL, "Should delete.");

  deleted = (bstring)Hashmap_delete(map, &test2);
  mu_assert(deleted != NULL, "Got NULL on delete.");
  mu_assert(deleted == &expect2, "Should get test2");
  result = Hashmap_get(map, &test2);
  mu_assert(result == NULL, "Should delete.");

  deleted = (bstring)Hashmap_delete(map, &test3);
  mu_assert(deleted != NULL, "Got NULL on deleted")

  // // orginal
  //   // mu_assert(deleted == &expect3, "Should get test3.");
  // // alternative:
    mu_assert(deleted == &expect1, "Should get test1.");

  result = Hashmap_get(map, &test3);
  mu_assert(result == NULL, "Should delete.");

  // wrong?

  // deleted = (bstring)Hashmap_delete(map, &test3);
  // mu_assert(deleted != NULL, "Got NULL on delete.");
  // mu_assert(deleted == &expect3, "Should get test3");
  // deleted = (bstring)Hashmap_delete(map, &test3);
  // mu_assert(deleted != NULL, "Got NULL on delete.");
  // mu_assert(deleted == &expect1, "Should get test1");
  // result = Hashmap_get(map, &test3);
  // mu_assert(result == NULL, "Should delete.");


  return NULL;
}


char *all_tests()
{
  mu_suite_start();

  mu_run_test(test_create);
  mu_run_test(test_get_set);
  mu_run_test(test_traverse);
  mu_run_test(test_delete);
  mu_run_test(test_destroy);

  return NULL;
}

RUN_TESTS(all_tests);