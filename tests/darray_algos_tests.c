#include "miniunit.h"
#include "darray_algos.h"

int testcmp(char **a, char **b)
{
  return strcmp(*a, *b);
}

DArray *create_words()
{
  DArray *result = DArray_create(0, 5);
  char *words[] = {"asdfasd", "werwar", "13234", "asdfasfd", "oioj"};
  int i = 0;

  for(i = 0; i < 5; i++) {
    DArray_push(result, words[i]);
  }

  return result;
}

int is_sorted(DArray *array)
{
  int i = 0;

  for(i = 0; i < DArray_count(array) - 1; i++) {
    if(strcmp(DArray_get(array, i), DArray_get(array, i+1)) > 0) {
      return 0;
    }
  }

  return 1;
}

char *run_sort_test(int (*func)(DArray *, DArray_compare), const char *name)
{
  DArray *words = create_words();
  mu_assert(!is_sorted(words), "Words should start not sorted");

  debug("--- Testing %s sorting algorithm", name);
  int rc = func(words, (DArray_compare)testcmp);
  mu_assert(rc == 0, "sort failed.");
  mu_assert(is_sorted(words), "didn't sort it");

  DArray_destroy(words);

  return NULL;
}

char *run_sort_test_choose_sort_type(DArray_sort_type sort_type)
{
  DArray *words = create_words();
  mu_assert(!is_sorted(words), "Words should start not sorted");

  int rc = DArray_sort(words, sort_type, (DArray_compare)testcmp);
  mu_assert(rc == 0, "sort failed.");
  mu_assert(is_sorted(words), "didn't sort it");

  DArray_destroy(words);

  return NULL;
}

char *test_qusort()
{
  return run_sort_test(DArray_qusort, "qusort");
}


char *test_lcthw_mergesort()
{
  return run_sort_test(DArray_lcthw_mergesort, "lcthw_mergesort");
}

char *test_choose_sort_type_qusort()
{
  return run_sort_test_choose_sort_type(qusort);
}

char *test_choose_sort_type_lcthw_mergesort()
{
  return run_sort_test_choose_sort_type(lcthw_mergesort);
}

char *test_add_and_sort()
{
  DArray *words = create_words();
  mu_assert(!is_sorted(words), "Words should start not sorted");

  int rc = DArray_add_sort(words, "next_element", qusort, (DArray_compare)testcmp);
  mu_assert(rc == 0, "sort failed.");
  mu_assert(is_sorted(words), "didn't sort it");

  DArray_destroy(words);

  return NULL;
}

char *test_daaray_find()
{
  DArray *words = create_words();
  mu_assert(!is_sorted(words), "Words should start not sorted");

  // sort it first

  int rc = DArray_add_sort(words, "next_element", qusort, (DArray_compare)testcmp);
  mu_assert(rc == 0, "sort failed.");
  mu_assert(is_sorted(words), "didn't sort it");

  int result = DArray_find(words, "asdfasfd", (DArray_compare)testcmp);
  // 0 is with numbers, 1 the similar but shorted word and 2 is correct
  mu_assert(result == 2, "Error with finding: should have found, but returned wrong index");

  result = DArray_find(words, "wtf", (DArray_compare)testcmp);
  mu_assert(result == -1, "Error with finding: should not have found, but returned index");

  return NULL;
}

// char *test_lcthw_heapsort()
// {
//   return run_sort_test(DArray_lcthw_heapsort, "lcthw_heapsort");
// }

char *all_tests()
{
  mu_suite_start();

  mu_run_test(test_qusort);
  mu_run_test(test_lcthw_mergesort);
  // mu_run_test(test_lcthw_heapsort);
  mu_run_test(test_choose_sort_type_qusort);
  mu_run_test(test_choose_sort_type_lcthw_mergesort);
  mu_run_test(test_add_and_sort);
  mu_run_test(test_daaray_find);

  return NULL;
}

RUN_TESTS(all_tests);