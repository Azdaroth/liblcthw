#include "bstrlib.h"
#include "hashmap.h"
#include "hashmap_algos.h"
#include "darray.h"
#include "miniunit.h"

struct tagbstring test1 = bsStatic("test data 1");
struct tagbstring test2 = bsStatic("test data 2");
struct tagbstring test3 = bsStatic("test data 3");

char *test_nfv1a()
{
  uint32_t hash = Hashmap_fnv1a_hash(&test1);
  mu_assert(hash != 0, "Bad hash.");

  hash = Hashmap_fnv1a_hash(&test2);
  mu_assert(hash != 0, "Bad hash.");

  hash = Hashmap_fnv1a_hash(&test3);
  mu_assert(hash != 0, "Bad hash.");

  return NULL;
}

char *test_adler32()
{
  uint32_t hash = Hashmap_adler32_hash(&test1);
  mu_assert(hash != 0, "Bad hash.");

  hash = Hashmap_adler32_hash(&test2);
  mu_assert(hash != 0, "Bad hash.");

  hash = Hashmap_adler32_hash(&test3);
  mu_assert(hash != 0, "Bad hash.");

  return NULL;
}

char *test_djb()
{
  uint32_t hash = Hashmap_djb_hash(&test1);
  mu_assert(hash != 0, "Bad hash.");

  hash = Hashmap_djb_hash(&test2);
  mu_assert(hash != 0, "Bad hash.");

  hash = Hashmap_djb_hash(&test3);
  mu_assert(hash != 0, "Bad hash.");

  return NULL;
}

char *test_jenkins()
{
  uint32_t hash = Hashmap_jenkins_hash(&test1);
  mu_assert(hash != 0, "Bad hash.");

  hash = Hashmap_jenkins_hash(&test1);
  mu_assert(hash != 0, "Bad hash.");

  hash = Hashmap_jenkins_hash(&test1);
  mu_assert(hash != 0, "Bad hash.");

  return NULL; 
}

char *test_azdaroth()
{
  uint32_t hash = Hashmap_azdaroth_hash(&test1);
  mu_assert(hash != 0, "Bad hash.");

  hash = Hashmap_azdaroth_hash(&test2);
  mu_assert(hash != 0, "Bad hash.");

  hash = Hashmap_azdaroth_hash(&test3);
  mu_assert(hash != 0, "Bad hash.");

  return NULL; 
}

#define BUCKETS 100
#define BUFFER_LEN 20
#define NUM_KEYS BUCKETS * 100
enum { ALGO_FNV1A, ALGO_ADLER32, ALGO_DJB, ALGO_JENKINS, ALGO_AZDAROTH };

int gen_keys(DArray *keys, int num_keys)
{
  int i = 0;
  FILE *urand = fopen("/dev/urandom", "r");
  check(urand != NULL, "Failed to open /dev/urandom");

  struct bStream *stream = bsopen((bNread)fread, urand);
  check(stream != NULL, "Failed to open /dev/urandom.");

  bstring key = bfromcstr("");
  int rc = 0;

  // FNV1a histrogram
  for(i = 0; i < num_keys; i++) {
    rc = bsread(key, stream, BUFFER_LEN);
    check(rc >= 0, "Failed to read from /dev/urandom.");

    DArray_push(keys, bstrcpy(key));
  }

  bsclose(stream);
  fclose(urand);
  return 0;

error:
  return -1;
}

void destroy_keys(DArray *keys)
{
  int i = 0;
  for(i = 0; i < NUM_KEYS; i++) {
    bdestroy(DArray_get(keys, i));
  }

  DArray_destroy(keys);
}

void fill_distribution(int *stats, DArray *keys, Hashmap_hash hash_func)
{
  int i = 0;
  uint32_t hash = 0;

  for(i = 0; i < DArray_count(keys); i++) {
    hash = hash_func(DArray_get(keys, i));
    stats[hash % BUCKETS] += 1;
  }
}

char *test_distribution()
{
  int i = 0;
  int stats[5][BUCKETS] = {{0}};
  DArray *keys = DArray_create(0, NUM_KEYS);

  mu_assert(gen_keys(keys, NUM_KEYS) == 0, "Failed to generate random keys.");

  fill_distribution(stats[ALGO_FNV1A], keys, Hashmap_fnv1a_hash);
  fill_distribution(stats[ALGO_ADLER32], keys, Hashmap_adler32_hash);
  fill_distribution(stats[ALGO_DJB], keys, Hashmap_djb_hash);
  fill_distribution(stats[ALGO_JENKINS], keys, Hashmap_jenkins_hash);
  fill_distribution(stats[ALGO_AZDAROTH], keys, Hashmap_azdaroth_hash);

  fprintf(stderr, "FNV\tA32\tDJB\tJENKINS\tAZDAROTH\n");

  for(i = 0; i < BUCKETS; i++) {
    fprintf(stderr, "%d\t%d\t%d\t%d\t%d\n",
      stats[ALGO_FNV1A][i],
      stats[ALGO_ADLER32][i],
      stats[ALGO_DJB][i], 
      stats[ALGO_JENKINS][i],
      stats[ALGO_AZDAROTH][i]);
  }

  destroy_keys(keys);

  return NULL;
}

char *all_tests()
{
  mu_suite_start();

  mu_run_test(test_nfv1a);
  mu_run_test(test_adler32);
  mu_run_test(test_djb);
  mu_run_test(test_jenkins);
  mu_run_test(test_azdaroth);
  mu_run_test(test_distribution);

  return NULL;
}

RUN_TESTS(all_tests);