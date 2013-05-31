#include "miniunit.h"
#include "ringbuffer_posix.h"

static ring_buffer *buffer = NULL;
static char target[50] = {'\0'};

char *test_create()
{

  int order = 20;
  buffer = ring_buffer_create(order);

  mu_assert(buffer != NULL, "Failed to create buffer.");
  // 2**20
  mu_assert(buffer->count_bytes == 1048576, "Wrong count_bytes assigned.");
  mu_assert(buffer->write_offset_bytes == 0, "Write Offset bytes should be 0.");
  mu_assert(buffer->read_offset_bytes == 0, "Read Offset bytes should be 0.");

  return NULL;
}

char *test_destroy()
{
  ring_buffer_destroy(buffer);

  return NULL;
}

char *test_write()
{

  unsigned long rc;
  char *data = "WTF";
  unsigned long size = (unsigned long)sizeof(data);
  unsigned long address = buffer->address;

  rc = ring_buffer_write_address(buffer, data, size);
  mu_assert(buffer->read_offset_bytes == size, "Wrong read read_offset_bytes after writing");
  mu_assert(rc == address, "Wring written bytes value.");

  rc = ring_buffer_count_bytes(buffer);
  mu_assert(rc == -size, "Wrong count bytes value");

  rc = ring_buffer_free_bytes(buffer);
  mu_assert(rc == (1048576 + size), "Wrong free bytes value.");

  rc = ring_buffer_read_address(buffer, target, size);

  printf("%s\n", (char *)target);

  return NULL;
}

char *test_clear()
{
  ring_buffer_clear(buffer);

  mu_assert(buffer->write_offset_bytes == 0, "Write Offset bytes should be 0.");
  mu_assert(buffer->read_offset_bytes == 0, "Read Offset bytes should be 0.");

  return NULL;
}

char *all_tests()
{
  mu_suite_start();

  mu_run_test(test_create);
  mu_run_test(test_write);
  mu_run_test(test_clear);
  mu_run_test(test_destroy);

  return NULL;
}

RUN_TESTS(all_tests);
