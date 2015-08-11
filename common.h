#include <stdint.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define WANT_HEADER
#include FN
#undef WANT_HEADER

typedef struct
{
  char binary_name[48];
  uint64_t size, capacity;
  rec items[];
} header;
