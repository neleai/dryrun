#define _GNU_SOURCE
#include <dlfcn.h>
#ifdef WANT_HEADER


typedef struct
{
  uint64_t s;
  uint64_t n;
  uint64_t c;
  uint64_t success;
  uint64_t cache_latency;
  char pattern[64];
  char ifs[64];
} rec;

# define REPLAY_SETUP \
  char *tmp = malloc (2000000); \
  memset (tmp, 1, 2000000); \
  tmp = tmp + 4096; \
  tmp = tmp - ((uint64_t) tmp) % 4096; 



# ifdef SHOW
#  define REPLAY_ITERATION(rec) printf("pattern: %s ifs: %s\n",rec.pattern, rec.ifs);
# elif defined SUMMARY

#include "summary_common.h"

# else
#  define REPLAY_ITERATION(rec) \
  if (rec.n < 1000000) \
    { \
      char *start = tmp + (((unsigned long) rec.s) % (1 << 20)); \
      if (rec.c == 1) \
      rec.c = 2; \
      start[rec.n] = rec.success ? rec.c : 0; \
      ret += (uintptr_t) strchr (start, rec.c); \
      start[rec.n] = 1; \
    }
# endif

#endif

#ifdef WANT_SUMMARY
#define NO_SUCCESS
#include "summary_common.h"
#endif


#ifdef WANT_RECORD
int wordexp (const char *s, void *r,int flag)
{
  rec *item = get_item();
  strncpy(item->pattern,s, 63);
  if (getenv("IFS"))
  strncpy(item->ifs,getenv("IFS"), 63);

  int (*f)(const char *, void *,int);
  f = dlsym(RTLD_NEXT, "wordexp");
  return f(s,r,flag);
}
#endif
