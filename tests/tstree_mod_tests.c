#include "tstree_mod.h"
#include "miniunit.h"
#include "bstrlib.h"
#include <string.h>
#include <assert.h>

TSTree *node = NULL;
char *valueA = "VALUEA";
char *valueB = "VALUEB";
char *value2 = "VALUE2";
char *value4 = "VALUE4";
char *reverse = "VALUER";
int traverse_count = 0;

struct tagbstring test1 = bsStatic("TEST");
struct tagbstring test2 = bsStatic("TEST2");
struct tagbstring test3 = bsStatic("TSET");
struct tagbstring test4 = bsStatic("T");

char *test_insert()
{
  node = TSTree_insert(node, bdata(&test1), blength(&test1), valueA);
  mu_assert(node != NULL, "Failed to insert into tst.");

  node = TSTree_insert(node, bdata(&test2), blength(&test2), value2);
  mu_assert(node != NULL, "Failed to insert into tst with second name.");

  node = TSTree_insert(node, bdata(&test3), blength(&test3), reverse);
  mu_assert(node != NULL, "Failed to insert into tst with reverse name.");

  node = TSTree_insert(node, bdata(&test4), blength(&test4), value4);
  mu_assert(node != NULL, "Failed to insert into tst with fourth name.");

  node = TSTree_insert(node, bdata(&test4), blength(&test4), value4);
  mu_assert(node != NULL, "Failed to insert into tst with fourth name as a duplicate.");


  return NULL;
}

char *test_search_exact()
{
  // tst returns the last one inserted

  DArray *res = TSTree_search(node, bdata(&test1), blength(&test1));
  mu_assert(DArray_first(res) == valueA, "Got the wrong value back, should get A not B.");

  res = TSTree_search(node, bdata(&test4), blength(&test4));
  mu_assert(DArray_count(res) == 2, "Should get duplicate.");

  // tst does not find if not exact
  res = TSTree_search(node, "TESTNO", strlen("TESTNO"));
  mu_assert(res == NULL, "Should not find anything");

  return NULL;
}

char *test_search_prefix()
{
  DArray *res = TSTree_search_prefix(node, bdata(&test1), blength(&test1));
  debug("result: %p, expected: %p", res, valueA);
  mu_assert(DArray_first(res) == valueA, "Got wrong valueA by prefix.");

  res = TSTree_search_prefix(node, bdata(&test1), 1);
  debug("result: %p, expected: %p", res, valueA);
  mu_assert(DArray_first(res) == value4, "Got wrong value4 for prefix of 1.");


  res = TSTree_search_prefix(node, "TE", strlen("TE"));
  mu_assert(DArray_first(res) != NULL, "Should find for short prefix");

  res = TSTree_search_prefix(node, "TE--", strlen("TE--"));
  mu_assert(DArray_first(res) != NULL, "Should find for partial prefix.");

  return NULL;
}

char *test_get_all_for_prefix()
{
  DArray *res = TSTree_get_all_for_prefix(node, "T", strlen("T"));

  mu_assert(res != NULL, "Search result should not be null");
  mu_assert(DArray_count(res) == 4, "Should find all 4 values");

  return NULL;
}


void TSTree_traverse_test_cb(DArray *values, void *data)
{
  assert(DArray_count(values) > 0 && "Should not get empty DArray as values.");
  assert(data == valueA && "Expecting valueA as the data.");
  traverse_count++;
}

char *test_traverse()
{
  traverse_count = 0;
  TSTree_traverse(node, TSTree_traverse_test_cb, valueA);
  debug("traverse count is: %d", traverse_count);
  mu_assert(traverse_count == 4, "Didn't find 4 keys.");

  return NULL;
}

char *test_destroy()
{
  TSTree_destroy(node);

  return NULL;
}

char *test_delete_values()
{

  int rc = TSTree_delete_values(node, bdata(&test1), blength(&test1));
  mu_assert(rc == 1, "Should delete values");

  DArray *res = TSTree_search(node, bdata(&test1), blength(&test1));
  mu_assert(DArray_empty(res), "Should be empty darray.");

  return NULL;
}

char *all_tests()
{
  mu_suite_start();

  mu_run_test(test_insert);
  mu_run_test(test_search_exact);
  mu_run_test(test_search_prefix);
  mu_run_test(test_get_all_for_prefix);
  // mu_run_test(test_get_keys);
  mu_run_test(test_traverse);
  mu_run_test(test_delete_values);
  mu_run_test(test_destroy);

  return NULL;
}

RUN_TESTS(all_tests);
