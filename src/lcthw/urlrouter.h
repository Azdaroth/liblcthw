#ifndef _lcthw_urlrouter
#define _lcthw_urlrouter

#include "tstree.h"
#include "bstrlib.h"

TSTree *add_route_data(TSTree *routes, bstring line);

TSTree *load_routes(const char *file);

bstring match_url(TSTree *routes, bstring url);

bstring read_line(const char *prompt);

void bdestroy_cb(void *value, void *ignored);

void destroy_routes(TSTree *routes);

#endif
