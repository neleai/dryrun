#ifdef WANT_HEADER


typedef struct
{
  uint64_t s;
  uint64_t n;
  uint64_t capa;
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
#  define REPLAY_ITERATION(rec) printf ("align %8i capa %8i capacity %8i character %c(%3i) found: %i latency %i\n", rec.s % 64, rec.n, rec.capa, isprint (rec.c) ? rec.c : ' ', rec.c, rec.success, rec.cache_latency)

# elif defined SUMMARY
#include "summary_common.h"

# else
#  define REPLAY_ITERATION(rec) \
  if (rec.n < 1000000) \
    { \
      char *start = tmp + (((unsigned long) rec.s) % (1 << 20)); \
      if (rec.c <= 1) \
      rec.c = 2; \
      start[rec.capa - rec.n] = rec.success ? rec.c : 0; \
      ret += (uintptr_t) memrchr (start, rec.c, rec.capa); \
      start[rec.capa - rec.n] = 1; \
    }
# endif

#endif

#ifdef WANT_SUMMARY
#define USES_CAPA
COUNTER(100,r.success,"\nsuceed: ");
COUNTER (200, r.c == '\0', "c == '\\0':");

#include "summary_common.h"
#endif


#ifdef WANT_RECORD
void *
memrchr (const void *_s, int _c, size_t capa)
{
  unsigned char *s = (unsigned char *) _s, c = _c;
  long i;
  rec *item = get_item();
  int latency = cache_probe (s);

  for (i = capa - 1; i >= 0 && s[i] != c; i--)
    ;


      item->cache_latency = latency;
      item->s = (uint64_t) s;
      item->c = c;
      item->n = capa - i;
      item->capa = capa;

      item->success = (i >= 0);

  return (void *)(i < 0 ? NULL : s + i);
}
#endif
