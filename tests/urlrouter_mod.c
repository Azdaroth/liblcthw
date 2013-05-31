#include "urlrouter_mod.h"


void *load_dl(char *lib_file)
{
  void *lib = dlopen(lib_file, RTLD_NOW);
  check(lib != NULL, "Failed to read library.");

  return lib;

error:
  return NULL;
}

Handler *Handler_create(bstring url, char *dl_name)
{
  Handler *handler = calloc(1, sizeof(Handler));
  check(handler != NULL, "Failed to create handler.");

  handler->url = url;
  handler->dl_name = dl_name;

  return handler;

error:
  return NULL;
}

void Handler_destroy(Handler *handler)
{
  if(handler != NULL) {
    free(handler);
  }
}

TSTree *add_route_data(TSTree *routes, bstring line)
{
  struct bstrList *data = bsplit(line, ' ');
  // eg. /hello && Hello

  check(data->qty == 2, "Line '%s' does not have 2 columns",
              bdata(line));


  // insert to routes, root is the first, the 2 and 3 is key anf length of key and value
  routes = TSTree_insert(routes,
                bdata(data->entry[0]), blength(data->entry[0]),
                bstrcpy(data->entry[1]));

  void *dl = load_dl(data->entry[1]);

  Handler *handler = Handler_create(bdata(data->entry[0]), dl);



  bstrListDestroy(data);

  return routes;

error:
  return NULL;
}

TSTree *load_routes(const char *file)
{
  TSTree *routes = NULL;
  bstring line = NULL;
  FILE *routes_map = 0;


  // open file
  routes_map = fopen(file, "r");
  check(routes_map != NULL, "Failed to open routes: %s", file);


  // get each line, trim it (remove whitespaces) and add routes
  while((line = bgets((bNgetc)fgetc, routes_map, '\n')) != NULL) {
    check(btrimws(line) == BSTR_OK, "Failed to trim line.");
    routes = add_route_data(routes, line);
    check(routes != NULL, "Failed to add route.");
    bdestroy(line);
  }

  fclose(routes_map);
  return routes;

error:
  if(routes_map) fclose(routes_map);
  if(line) bdestroy(line);

  return NULL;
}

bstring match_url(TSTree *routes, bstring url)
{
  bstring route = TSTree_search(routes, bdata(url), blength(url));

  if(route == NULL) {
    printf("No exact match found, trying prefix. \n");
    route = TSTree_search_prefix(routes, bdata(url), blength(url));
  }

  return route;
}

bstring read_line(const char *prompt)
{
  printf("%s", prompt);

  bstring result = bgets((bNgetc)fgetc, stdin, '\n');
  check_debug(result != NULL, "stdin closed.");

  check(btrimws(result) == BSTR_OK, "Failed to trim.");

  return result;

error:
  return NULL;
}

void bdestroy_cb(void *value, void *ignored)
{
  (void)ignored;
  bdestroy((bstring)value);
}

void destroy_routes(TSTree *routes)
{
  TSTree_traverse(routes, bdestroy_cb, NULL);
  TSTree_destroy(routes);
}

int main(int argc, char *argv[])
{

  bstring url = NULL;
  bstring route = NULL;
  TSTree *routes = NULL;
  check(argc == 2, "USAGE: urlor <urlfile>");

  routes = load_routes(argv[1]);
  check(routes != NULL, "Your route file has an error.");

  while(1) {
    url = read_line("URL> ");
    check_debug(url != NULL, "goodbye.");

    route = match_url(routes ,url);

    if(route) {
      printf("MATCH: %s == %s\n", bdata(url), bdata(route));
    } else {
      printf("FAIL: %s\n", bdata(url));
    }
  }


  destroy_routes(routes);
  return 0;

error:
  if(routes) destroy_routes(routes);
  return -1;
}
