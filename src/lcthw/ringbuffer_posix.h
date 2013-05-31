#ifndef _lcthw_ringbuffer_posix
#define _lcthw_ringbuffer_posix

#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct  ring_buffer
{
  void *address;

  unsigned long count_bytes;
  unsigned long write_offset_bytes;
  unsigned long read_offset_bytes;
} ring_buffer;

ring_buffer *ring_buffer_create(unsigned long order);

void ring_buffer_destroy(ring_buffer *buffer);

void ring_buffer_free(ring_buffer *buffer);

void *ring_buffer_write_address(ring_buffer *buffer, char *data, unsigned long length);

void ring_buffer_write_advance(ring_buffer *buffer, unsigned long count_bytes);

void *ring_buffer_read_address(ring_buffer *buffer, char *data, unsigned long length);

unsigned long ring_buffer_count_bytes(ring_buffer *buffer);

unsigned long ring_buffer_free_bytes(ring_buffer *buffer);

void ring_buffer_clear(ring_buffer *buffer);

#define buffer_ends(B) ((B)->address + (B)->write_offset_bytes)

#define buffer_starts(B) ((B)->address + (B)->read_offset_bytes)

#define commit_read(B, A) ((B)->read_offset_bytes = ((B)->read_offset_bytes - A))

#endif
