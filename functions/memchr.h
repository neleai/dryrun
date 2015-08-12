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

# else
#  define REPLAY_ITERATION(rec) \
  if (rec.n < 1000000) \
    { \
      char *start = tmp + (((unsigned long) rec.s) % (1 << 20)); \
      if (rec.c == 1) \
      rec.c = 2; \
      start[rec.n] = rec.success ? rec.c : 0; \
      ret += (uintptr_t) memchr (start, rec.c, rec.capa); \
      start[rec.n] = 1; \
    }
# endif

#endif

#ifdef WANT_SUMMARY
#define USES_CAPA
COUNTER(100,r.success,"\nsuceed: ");
COUNTER (300, r.c == '\0', "c == '\\0':");
#include "summary_common.h"
#endif



#ifdef WANT_RECORD
void *
memchr (const void *_s, int _c, size_t capa)
{
  unsigned char *s = (unsigned char *) _s, c = _c;
  long i;
  rec *item = get_item();


  int latency = cache_probe (s);

  for (i = 0; i < capa && s[i] != c; i++)
    ;


  item->cache_latency = latency;
  item->s = (uint64_t) s;
  item->c = c;
  item->n = i;
  item->capa = capa;
  item->success = (i != capa);

  return (void *)(i == capa ? NULL : s + i);
}
#endif
