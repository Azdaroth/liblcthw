#include "miniunit.h"
#include "list.h"
#include <assert.h>
#include <string.h>
#include "bstrlib.h"

static List *list = NULL;
char *test1 = "test1 data";
char *test2 = "test2 data";
char *test3 = "test3 data";

static List *list_copy_from = NULL;
static List *list_copy_to = NULL;
char *copy_test1 = "test1 copy";
char *copy_test2 = "test2 copy";

char *test_create()
{
  list = List_create();
  mu_assert(list != NULL, "Failed to create list.");

  return NULL;
}

char *test_destroy()
{
  List_clear_destroy(list);

  return NULL;

}

char *test_push_pop()
{
  List_push(list, test1);
  mu_assert(List_last(list) == test1, "Wrong last value.");

  List_push(list, test2);
  mu_assert(List_last(list) == test2, "Wrong last value.");

  List_push(list, test3);
  mu_assert(List_last(list) == test3, "Wrong last value.");
  mu_assert(List_count(list) == 3, "Wrong count on push.");

  char *val = List_pop(list);
  mu_assert(val == test3, "Wrong value on pop.");

  val = List_pop(list);
  mu_assert(val == test2, "Wrong value on pop.");

  val = List_pop(list);
  mu_assert(val == test1, "Wrong value on pop.");
  mu_assert(List_count(list) == 0, "Wrong count after pop.");

  return NULL;
}

char *test_unshift()
{
  List_unshift(list, test1);
  mu_assert(List_first(list) == test1, "Wrong first value.");

  List_unshift(list, test2);
  mu_assert(List_first(list) == test2, "Wrong first value.");

  List_unshift(list, test3);
  mu_assert(List_first(list) == test3, "Wrong first value.");
  mu_assert(List_count(list) == 3, "Wrong count on unshift.");

  return NULL;
}

char *test_remove()
{
  // we only need to test the middle remove case since push/shift
  // already tests the other cases

  char *val = List_remove(list, list->first->next);
  mu_assert(val == test2, "Wrong removed.");
  mu_assert(List_count(list) == 2, "Wrong count after remove.");
  mu_assert(List_first(list) == test3, "Wrong first after remove.");
  mu_assert(List_last(list) == test1, "Wrong last after remove.");

  return NULL;
}

char *test_shift()
{
  mu_assert(List_count(list) != 0, "Wrong count before shift.");

  char *val = List_shift(list);
  mu_assert(val == test3, "Wrong value on shift.");

  val = List_shift(list);
  mu_assert(val == test1, "Wrong value on shift.");
  mu_assert(List_count(list) == 0, "Wrong count after shift.");

  return NULL;
}

char *test_copy() {

  list_copy_from = List_create();
  mu_assert(list_copy_from != NULL, "Failed to create list.");
  list_copy_to = List_create();
  mu_assert(list_copy_to != NULL, "Failed to create list.");

  List_push(list_copy_to, copy_test1);
  List_push(list_copy_from, copy_test2);

  List_copy(list_copy_from, list_copy_to);
  mu_assert(List_last(list_copy_to) == copy_test2, "Wrong last value after copy");

  return NULL;
}


char *test_join() {
  mu_assert(bstricmp(List_join(list_copy_to), bfromcstr("test1 copy test2 copy")) == 0, "Error while joining");

  return NULL;
}

// char *test_split()
// {

//   char *string = "some random string";
//   int size = strlen(string);
//   List *list_split = split_to_list(string, size);
//   mu_assert(strcmp(List_last(list_split), "string") == 0, "Wrong last value after split.");  
//   mu_assert(strcmp(List_first(list_split), "some") == 0, "Wrong first value after split.");
//   mu_assert(list_split->count == 3, "Wrong count after split.");

//   return NULL;
// }

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_create);
  mu_run_test(test_push_pop);
  mu_run_test(test_unshift);
  mu_run_test(test_remove);
  mu_run_test(test_shift);
  mu_run_test(test_destroy);
  mu_run_test(test_copy);
  mu_run_test(test_join);
  // mu_run_test(test_split);

  return NULL;
}

RUN_TESTS(all_tests);