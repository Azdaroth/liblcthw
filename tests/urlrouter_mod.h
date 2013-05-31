#ifndef _lcthw_urlrouter_mod
#define _lcthw_urlrouter_mod

#include <dlfcn.h>
#include "tstree.h"
#include "bstrlib.h"
#include "darray.h"

typedef struct Handlers
{
  DArray *handlers;
} Handlers;

typedef struct Handler
{
  bstring url;
  char *dl_name;
} Handler;

Handler *Handler_create(bstring url, char *dl_name);

void Handler_destroy(Handler *handler);

void  *load_dl(char *lib_file);

TSTree *add_route_data(TSTree *routes, bstring line);

TSTree *load_routes(const char *file);

bstring match_url(TSTree *routes, bstring url);

bstring read_line(const char *prompt);

void bdestroy_cb(void *value, void *ignored);

void destroy_routes(TSTree *routes);

#endif
