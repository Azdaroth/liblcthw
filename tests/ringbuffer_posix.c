// #include "ringbuffer_posix.h"
// #include "dbg.h"
// #include <stdlib.h>

// #define report_exceptional_condition() abort()

// // Warning order should be at least 12 for Linux
// ring_buffer *ring_buffer_create(unsigned long order)
// {
//   char path[] = "/dev/shm/ring-buffer-XXXXXX";
//   int file_descriptior;
//   void *address;
//   int status;

//   file_descriptior = mkstemp(path);
//   if (file_descriptior < 0) report_exceptional_condition();

//   status = unlink(path);
//   if (status) report_exceptional_condition();

//   ring_buffer *buffer = calloc(1, sizeof(ring_buffer));

//   check(buffer != NULL, "Failed to create buffer.");

//   buffer->count_bytes = 1UL << order;
//   buffer->write_offset_bytes = 0;
//   buffer->read_offset_bytes = 0;

//   status = ftruncate(file_descriptior, buffer->count_bytes);
//   if (status) report_exceptional_condition();

//   buffer->address = mmap(NULL, buffer->count_bytes << 1, PROT_NONE,
//                                         MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

//   if (buffer->address == MAP_FAILED) report_exceptional_condition();

//   address = mmap(buffer->address, buffer->count_bytes, PROT_READ | PROT_WRITE,
//                             MAP_FIXED | MAP_SHARED, file_descriptior, 0);

//   if (address != buffer->address) report_exceptional_condition();

//   address = mmap(buffer->address + buffer->count_bytes, buffer->count_bytes,
//                             PROT_READ | PROT_WRITE, MAP_FIXED | MAP_SHARED, file_descriptior, 0);

//   if (address != buffer->address + buffer->count_bytes) report_exceptional_condition();

//   status = close(file_descriptior);
//   if (status) report_exceptional_condition();

//   return buffer;

// error:
//   return NULL;
// }

// void ring_buffer_destroy(ring_buffer *buffer)
// {
//   if(buffer) {
//     free(buffer);
//   }
// }

// void ring_buffer_free(ring_buffer *buffer)
// {
//   int status;

//   status = munmap(buffer->address, buffer->count_bytes << 1);
//   if (status) report_exceptional_condition();
// }

// void *ring_buffer_write_address(ring_buffer *buffer, char *data, unsigned long length)
// {
//   /*** void pointer arithmetic is a constraint violation. ***/

//     void *result = memcpy(buffer_ends(buffer), data, length);
//     check(result != NULL, "Failed to write data into buffer.");

//     ring_buffer_write_advance(buffer, length);

//     return buffer->address + buffer->write_offset_bytes;
// error:
//     return NULL;
// }

// void *ring_buffer_read_address(ring_buffer *buffer, char *data, unsigned long length)
// {

//   void *result = memcpy(data, buffer_starts(buffer), length);
//   check(result != NULL, "Failed to write data into buffer.");

//   commit_read(buffer, length);

//   return buffer->address + buffer->read_offset_bytes;
// error:
//   return NULL;
// }

// void ring_buffer_write_advance(ring_buffer *buffer, unsigned long count_bytes)
// {
//   buffer->read_offset_bytes += count_bytes;
//   if (buffer->read_offset_bytes >= buffer->count_bytes) {
//     buffer->read_offset_bytes -= buffer->count_bytes;
//     buffer->write_offset_bytes -= buffer->count_bytes;
//   }
// }

// unsigned long ring_buffer_count_bytes(ring_buffer *buffer)
// {
//   return buffer->write_offset_bytes - buffer->read_offset_bytes;
// }

// unsigned long ring_buffer_free_bytes(ring_buffer *buffer)
// {
//   return buffer->count_bytes - ring_buffer_count_bytes(buffer);
// }

// void ring_buffer_clear(ring_buffer *buffer)
// {
//   buffer->write_offset_bytes = 0;
//   buffer->read_offset_bytes = 0;
// }

