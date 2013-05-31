#include "miniunit.h"
#include "list_algos.h"
#include <assert.h>
#include <string.h>
#include <time.h>

char *values[] = {"XXXX", "1234", "abcd", "xjvef", "NDSS", "1746", "WTF", "Va1ue",  "WTF", "Va1ue", "WTF", "Va1ue", "XXXX", "1234", "abcd", "xjvef", "NDSS", "1746", "WTF", "Va1ue",  "WTF", "Va1ue", "WTF", "Va1ue", "XXXX", "1234", "abcd", "xjvef", "NDSS", "1746", "WTF", "Va1ue",  "WTF", "Va1ue", "WTF", "Va1ue", "XXXX", "1234", "abcd", "xjvef", "NDSS", "1746", "WTF", "Va1ue",  "WTF", "Va1ue", "WTF", "Va1ue", "XXXX", "1234", "abcd", "xjvef", "NDSS", "1746", "WTF", "Va1ue",  "WTF", "Va1ue", "WTF", "Va1ue", "XXXX", "1234", "abcd", "xjvef", "NDSS", "1746", "WTF", "Va1ue",  "WTF", "Va1ue", "WTF", "Va1ue", "XXXX", "1234", "abcd", "xjvef", "NDSS", "1746", "WTF", "Va1ue",  "WTF", "Va1ue", "WTF", "Va1ue", "XXXX", "1234", "abcd", "xjvef", "NDSS", "1746", "WTF", "Va1ue",  "WTF", "Va1ue", "WTF", "Va1ue", "XXXX", "1234", "abcd", "xjvef", "NDSS", "1746", "WTF", "Va1ue",  "WTF", "Va1ue", "WTF", "Va1ue", "XX", "4", "ab", "xjvef", "NS", "1746", "WTF", "Va",  "WT", "Va1e", "WTF", "Va1ue", "XXXX", "1234", "abcd", "xjvef", "NDSS", "1746", "WTF", "Va1ue",  "WTF", "Va1ue", "WTF", "Va1ue", "XXXX", "1234", "abcd", "xjvef", "NDSS", "1746", "WTF", "Va1ue",  "WTF", "Va1ue", "WTF", "Va1ue"};
#define NUM_VALUES 144

List *create_words()
{
  int i = 0;
  List *words = List_create();

  for (i = 0; i < NUM_VALUES; i++) {
    List_push(words, values[i]);
  }

  return words;
}

int is_sorted(List *words)
{
  LIST_FOREACH(words, first, next, cur) { 
    if(cur->next && strcmp(cur->value, cur->next->value) > 0) { 
      debug("%s %s", (char *)cur->value, (char *)cur->next->value);
      return 0;
    }
  }

  return 1;
}

char *test_bubble_sort()
{
  List *words = create_words();

  // should work on a list that needs sorting
  int rc = List_bubble_sort(words, (List_compare)strcmp);
  mu_assert(rc == 0, "Bubble sort failed.");
  mu_assert(is_sorted(words), "Words are not sorted after bubble sort.");

  // should work on an already sorted list
  rc = List_bubble_sort(words, (List_compare)strcmp);
  mu_assert(rc == 0, "Bubble sort of already sorted failed.");
  mu_assert(is_sorted(words), "Words should be sort if already bubble sorted.");

  List_destroy(words);

  // should work on an empty list
  words = List_create(words);
  rc = List_bubble_sort(words, (List_compare)strcmp);
  mu_assert(rc == 0, "Bubble sort failed on empty list.");
  mu_assert(is_sorted(words), "Words should be sorted if empty");

  List_destroy(words);

  return NULL;
}

char *test_merge_sort()
{
  List *words = create_words();

  // should work on a list that needs sorting
  List *res = List_merge_sort(words, (List_compare)strcmp);
  mu_assert(is_sorted(res), "Words are not sorted after merge sort.");

  List *res2 = List_merge_sort(res, (List_compare)strcmp);
  mu_assert(is_sorted(res), "Should still be sorted after merge sort.");
  List_destroy(res2);
  List_destroy(res);

  List_destroy(words);
  return NULL;
}

char *test_insert_sorted()
{

  List *list = List_create();

  list = List_insert_sorted(list, "b", (List_compare)strcmp);
  list = List_insert_sorted(list, "c", (List_compare)strcmp);
  list = List_insert_sorted(list, "a", (List_compare)strcmp);

  mu_assert(strcmp(list->first->value, "a") == 0, "Wrong first value");
  mu_assert(strcmp(list->first->next->value, "b") == 0, "Wrong second value");
  mu_assert(strcmp(list->last->value, "c") == 0, "Wrong last value");


  return NULL;
}


char *all_tests()
{

  clock_t start_time, end_time;
  double duration;

  time_t start, stop;
  double diff;

  mu_suite_start();

  start_time = clock();
  time(&start);
  mu_run_test(test_bubble_sort);
  time(&stop);
  end_time = clock();
  diff = difftime(stop, start);

  duration  = ((double)(end_time - start_time))/CLOCKS_PER_SEC;

  printf("Bubble Sort took: %.64lf time.\n",  diff);

  start_time = clock();
  mu_run_test(test_merge_sort);
  end_time = clock();

  duration  = (float)(end_time - start_time)/CLOCKS_PER_SEC;

  printf("Merge Sort took: %lf time.\n",  duration);

  mu_run_test(test_insert_sorted);

  return NULL;
}

RUN_TESTS(all_tests);