#ifdef WANT_HEADER


typedef struct
{
  uint64_t s;
  uint64_t n;
  uint64_t c;
  uint64_t success;
  uint64_t cache_latency;
} rec;

# define REPLAY_SETUP \
  char *tmp = malloc (2000000); \
  memset (tmp, 1, 2000000); \
  tmp = tmp + 4096; \
  tmp = tmp - ((uint64_t) tmp) % 4096; 


# ifdef SHOW
#  define REPLAY_ITERATION(rec) printf ("align %8i size %8i character %c(%3i) found: %i latency %i\n", rec.s % 64, rec.n, isprint (rec.c) ? rec.c : ' ', rec.c, rec.success, rec.cache_latency)
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
      ret += (uintptr_t) strchrnul (start, rec.c); \
      start[rec.n] = 1; \
    }
# endif

#endif

#ifdef WANT_SUMMARY
#define NO_SUCCESS
#include "summary_common.h"
#endif

#ifdef WANT_RECORD


char *
strchrnul (const char *_s, int _c)
{
  unsigned char *s = (unsigned char *) _s, c = _c;
  long i;
  rec *item = get_item();
  int latency = cache_probe (s);

  for (i = 0; s[i] && s[i] != c; i++)
    ;

      item->cache_latency = latency;
      item->s = (uint64_t) s;
      item->c = c;
      item->n = i;
      item->success = (s[i] == c);
  return s + i;
}
#endif
