#include "miniunit.h"
#include "ringbuffer.h"

static RingBuffer *buffer = NULL;
static RingBuffer *buffer_2 = NULL;
static char target[50] = {'\0'};

char *test_create()
{
  buffer = RingBuffer_create(50);
  mu_assert(buffer != NULL, "Ring buffer not created.");
  mu_assert(buffer->length == 51, "Wrong length set.");
  mu_assert(buffer->start == 0, "Wrong start value.");
  mu_assert(buffer->end == 0, "Wrong end value.");
  mu_assert(buffer->buffer != NULL, "Wrong buffer value.");

  buffer_2 = RingBuffer_create(5);
  mu_assert(buffer_2 != NULL, "Ring buffer not created.");
  mu_assert(buffer_2->length == 6, "Wrong length set.");
  mu_assert(buffer_2->start == 0, "Wrong start value.");
  mu_assert(buffer_2->end == 0, "Wrong end value.");
  mu_assert(buffer_2->buffer != NULL, "Wrong buffer value.");

  return NULL;
}


char *test_destroy()
{
  RingBuffer_destroy(buffer);
  RingBuffer_destroy(buffer_2);

  return NULL;
}


char *test_read_write()
{
  mu_assert(RingBuffer_available_data(buffer) == 0, "Wrong data count.");
  mu_assert(RingBuffer_available_space(buffer) == 50, "Wrong free slots count.");
  mu_assert(RingBuffer_empty(buffer) == 1, "RingBuffer should have been empty.");
  mu_assert(RingBuffer_full(buffer) == 0, "RingBuffer should not have been full.");

  int rc = 0;
  char *data = "WTF";
  char *data_2 = "DATA";
  int  size = sizeof(data);
  int size_2 = sizeof(data_2);

  rc = RingBuffer_write(buffer, data, size);
  mu_assert(rc == size, "Failed to write to buffer.");

  mu_assert(RingBuffer_available_data(buffer) == size, "Wrong data count.");
  mu_assert(RingBuffer_available_space(buffer) == (50 - size), "Wrong free slots count.");

  rc = RingBuffer_write(buffer, data_2, size_2);
  mu_assert(rc == size_2, "Failed to write to buffer.");

  mu_assert(RingBuffer_available_data(buffer) == size + size_2, "Wrong data count.");
  mu_assert(RingBuffer_available_space(buffer) == (50 - size - size_2), "Wrong free slots count.");

  mu_assert(RingBuffer_empty(buffer) == 0, "RingBuffer should not be empty.");
  mu_assert(RingBuffer_full(buffer) == 0, "RingBuffer should not be full.");

  rc = RingBuffer_read(buffer, target, size);
  mu_assert(rc == size, "Failed to read from buffer");

  mu_assert(strcmp(data, (char*)target) == 0, "Wrong data got from buffer.");

  mu_assert(RingBuffer_empty(buffer) == 0, "RingBuffer should not be empty.");
  mu_assert(RingBuffer_full(buffer) == 0, "RingBuffer should not be full.");

  rc = RingBuffer_read(buffer, target, size_2);
  mu_assert(rc == size_2, "Failed to read from buffer");

  mu_assert(RingBuffer_empty(buffer) == 1, "RingBuffer should have been empty.");
  mu_assert(RingBuffer_full(buffer) == 0, "RingBuffer should not have been full.");

  mu_assert(RingBuffer_available_data(buffer) == 0, "Wrong data count.");
  mu_assert(RingBuffer_available_space(buffer) == 50, "Wrong free slots count.");

  mu_assert(strcmp(data_2, (char*)target) == 0, "Wrong data got from buffer.");

  return NULL;
}

char *test_commits()
{

  char *data_2 = "DATA";
  int size_2 = sizeof(data_2);

  RingBuffer_commit_write(buffer, size_2);

  mu_assert(RingBuffer_available_data(buffer) == size_2, "Wrong data count.");
  mu_assert(RingBuffer_available_space(buffer) == (50-size_2), "Wrong free slots count.");

  RingBuffer_commit_write(buffer, size_2);

  mu_assert(RingBuffer_available_data(buffer) == 2*size_2, "Wrong data count.");
  mu_assert(RingBuffer_available_space(buffer) == (50-2*size_2), "Wrong free slots count.");

  RingBuffer_commit_read(buffer, size_2);

  mu_assert(RingBuffer_available_data(buffer) == size_2, "Wrong data count.");
  mu_assert(RingBuffer_available_space(buffer) == (50-2*size_2), "Wrong free slots count.");

  RingBuffer_commit_read(buffer, size_2);

  mu_assert(RingBuffer_available_data(buffer) == 0, "Wrong data count.");
  mu_assert(RingBuffer_available_space(buffer) == (50-2*size_2), "Wrong free slots count.");

  return NULL;
}

char *test_gets_and_get_all()
{

  int rc = 0;
  char *data_2 = "DATA";
  int size_2 = sizeof(data_2);

  rc = RingBuffer_write(buffer, data_2, size_2);
  mu_assert(rc == size_2, "Failed to write to buffer.");

  rc = RingBuffer_write(buffer, data_2, size_2);
  mu_assert(rc == size_2, "Failed to write to buffer.");

  rc = RingBuffer_write(buffer, data_2, size_2);
  mu_assert(rc == size_2, "Failed to write to buffer.");

  mu_assert(RingBuffer_available_data(buffer) == 3*size_2, "Wrong data count.");

  RingBuffer_gets(buffer, size_2);

  mu_assert(RingBuffer_available_data(buffer) == 2*size_2, "Wrong data count.");
  mu_assert(RingBuffer_available_space(buffer) == 50 -3*size_2, "Wrong free slots count.");

  RingBuffer_get_all(buffer);

  mu_assert(RingBuffer_available_data(buffer) == 0, "Wrong data count.");
  mu_assert(RingBuffer_available_space(buffer) == 50 -3*size_2, "Wrong free slots count.");

  return NULL;
}


char *all_tests()
{
  mu_suite_start();

  mu_run_test(test_create);
  mu_run_test(test_read_write);
  mu_run_test(test_commits);
  mu_run_test(test_gets_and_get_all);
  mu_run_test(test_destroy);

  return NULL;
}

RUN_TESTS(all_tests);
