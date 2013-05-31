#undef NDEBUG
#include <stdlib.h>
#include <sys/select.h>
#include <stdio.h>
#include "ringbuffer.h"
#include "dbg.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>

struct tagbstring NL = bsStatic("\n");
struct tagbstring CRLF = bsStatic("\r\n");
static int sub;

int nonblock(int fd)
{

  // getflags
  int flags = fcntl(fd, F_GETFL, 0);
  check(flags >= 0, "Invalid flags on nonblock.");


  // set flags that were got above + nonblock
  int rc = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
  check(rc == 0, "Can't see nonblocking.");

  return 0;
error:
  return -1;
}

int client_connect(char *host, char *port)
{

  check(host != NULL, "Host cannot be NULL.");
  check(port != NULL, "Port cannot be NULL.");

  int rc = 0;
  struct addrinfo *addr  = NULL;

  // fills addr struct with data
  rc = getaddrinfo(host, port, NULL, &addr);
  check(rc == 0, "Failed to lookup %s:%s", host, port);

  // returns fd based on passed args (IPV4 etc.)
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  check(sock >= 0, "Cannot create a socket.");

  // does, what is says
  rc = connect(sock, addr->ai_addr, addr->ai_addrlen);
  check(rc == 0, "Connect failed.");

  rc = nonblock(sock);
  check(rc == 0, "Can't set nonblocking.");

  freeaddrinfo(addr);
  return sock;

error:
  freeaddrinfo(addr);
  return -1;
}


// read stuff and saves in buffer
int read_some(RingBuffer *buffer, int fd, int is_socket)
{

  check(buffer != NULL, "Buffer cannot be NULL.");
  check(fd >= 0, "File descriptor can't be negative");
  check(is_socket == 0  || is_socket == 1, "is_socket must be boolean.");

  int rc = 0;

  if(RingBuffer_available_data(buffer) == 0) {
    buffer->start = buffer->end = 0;
  }

  if(is_socket) {
    rc = recv(fd, RingBuffer_starts_at(buffer), RingBuffer_available_space(buffer), 0);
  } else {
    rc = read(fd, RingBuffer_starts_at(buffer), RingBuffer_available_space(buffer));
  }

  check(rc >= 0, "Failed to read from fd: %d", fd);

  RingBuffer_commit_write(buffer, rc);

  return rc;
error:
  return -1;
}


// write to stdout - means reading from buffer
int write_some(RingBuffer *buffer, int fd, int is_socket)
{

  check(buffer != NULL, "Buffer cannot be NULL.");
  check(fd >= 0, "File descriptor can't be negative");
  check(is_socket == 0  || is_socket == 1, "is_socket must be boolean.");

  int rc = 0;
  bstring data = RingBuffer_get_all(buffer);

  check(data != NULL, "Failed to get from the buffer.");

  if (sub == 1) {
    check(bfindreplace(data, &NL, &CRLF, 0) == BSTR_OK, "Failed to replace NL.");
  }

  if(is_socket) {
    rc = send(fd, bdata(data), blength(data), 0);
  } else {
    rc = write(fd, bdata(data), blength(data));
  }

  check(rc == blength(data), "Failed to write everything to fd: %d.", fd);
  bdestroy(data);

  return rc;
error:
  return -1;
}

int main(int argc, char *argv[])
{
  fd_set allreads;
  fd_set readmask;

  int opt;
  sub = 1;

   check(argc >=3, "USAGE: netclient host port OPT.");

  while((opt = getopt(argc, argv, ":s:n:h")) != -1) {
      switch(opt) {
        case 's':
          printf("Will substitute to \\r\\n \n");
          sub = 1;
          break;
        case 'n':
          sub = 0;
          printf("Will not substitute to \\r\\n \n");
          break;
        case 'h':
          printf("-s: subsitute \\n to \\r\\n, -n: does not, -h: help\n");
          exit(0);
        default:
          printf("Will do \\r\\n \n");
          break;
      }
      break;
  }

  int socket = 0;
  int rc = 0;
  RingBuffer *in_rb = RingBuffer_create(1024 * 10);
  RingBuffer *sock_rb = RingBuffer_create(1024 * 10);

  socket = client_connect(argv[1], argv[2]);
  check(socket >= 0, "connect to %s:%s failed.", argv[1], argv[2]);

  FD_ZERO(&allreads);
  FD_SET(socket, &allreads);
  FD_SET(0, &allreads);

  printf("Started...\n");

  while(1) {
    readmask = allreads;
    // waits until ready for non-blocking
    rc = select(socket + 1, &readmask, NULL, NULL, NULL);
    check(rc >= 0, "select failed.");

    if(FD_ISSET(0, &readmask)) {
      printf("Type stuff:\n");
      rc = read_some(in_rb, 0, 0);
      check_debug(rc != -1, "Failed to read from stdin.");
    }

    if(FD_ISSET(socket, &readmask)) {
      printf("Reading from socket\n");
      rc = read_some(sock_rb, socket, 0);
      check_debug(rc != -1, "Failed to read from socket.");
    }

    while(!RingBuffer_empty(sock_rb)) {
      printf("Writing to STDOUT\n");
      rc = write_some(sock_rb, 1, 0);
      check_debug(rc != -1, "Failed to write to stdout.");
    }

    while(!RingBuffer_empty(in_rb)) {
      printf("Writing to buffer.\n");
      rc = write_some(in_rb, socket, 1);
      check_debug(rc != -1, "Failed to write to socket.");
    }

  }

  printf("END\n");

  return 0;
error:
  return -1;
}
