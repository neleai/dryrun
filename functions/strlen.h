#ifdef WANT_HEADER


typedef struct
{
  uint64_t s;
  uint64_t n;
  uint64_t cache_latency;
} rec;


# define REPLAY_SETUP \
  char *tmp = malloc (2000000); \
  memset (tmp, 1, 2000000); \
  tmp = tmp + 4096; \
  tmp = tmp - ((uint64_t) tmp) % 4096; 


# ifdef SHOW
#  define REPLAY_ITERATION(rec) printf ("align %8i size %8i latency %i\n", rec.s % 64, rec.n, rec.cache_latency)

# else
#  define REPLAY_ITERATION(rec) \
  if (rec.n < 1000000) \
    { \
      char *start = tmp + (((unsigned long) rec.s) % (1 << 20)); \
      start[rec.n] = 0; \
      ret += (uintptr_t) strlen (start); \
      start[rec.n] = 1; \
    }
# endif
#endif

#ifdef WANT_SUMMARY
#define NO_SUCCESS
#include "summary_common.h"
#endif

#ifdef WANT_RECORD
size_t
strlen (const char *_s)
{
  unsigned char *s = (unsigned char *) _s;
  long i;
  rec *item = get_item();
  int latency = cache_probe (s);

  for (i = 0; s[i] != 0; i++)
    ;

      item->cache_latency = latency;
      item->s = (uint64_t) s;
      item->n = i;
  return i;
}
#endif
