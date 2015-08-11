#ifdef WANT_HEADER


typedef struct
{
  uint64_t s;
  uint64_t s2;
  uint64_t n;
  uint64_t capa;
  uint64_t success;
  uint64_t cache_latency;
  uint64_t cache_latency2;

} rec;

# define REPLAY_SETUP \
  char *tmp = malloc (2000000); \
  memset (tmp, 1, 2000000); \
  tmp = tmp + 4096; \
  tmp = tmp - ((uint64_t) tmp) % 4096; \
  char *tmp2 = malloc (2000000); \
  memset (tmp2, 1, 2000000); \
  tmp2 = tmp2 + 4096; \
  tmp2 = tmp2 - ((uint64_t) tmp2) % 4096; 

# ifdef SHOW
#  define REPLAY_ITERATION(rec) printf ("salign %4i s2align %4i size %8i capacity %8i success: %i latency %4i %4i\n", rec.s % 64,rec.s2 % 64, rec.n, rec.capa,  rec.success, rec.cache_latency, rec.cache_latency2);

# elif defined SUMMARY
# define USES_CAPA
# include "summary_cmp_common.h"

# else
#  define REPLAY_ITERATION(rec) \
  if (rec.n < 1000000) \
    { \
      char *start1 = tmp + (((unsigned long) rec.s) % (1 << 20)); \
      char *start2 = tmp2 + (((unsigned long) rec.s2) % (1 << 20)); \
      start1[rec.n] = rec.success ? 2 : 1; \
      ret += (uintptr_t) memcmp (start1, start2, rec.capa); \
      start1[rec.n] = 1; \
    }
# endif

#endif

#ifdef WANT_SUMMARY
#include "summary_cmp_common.h"
#endif


#ifdef WANT_RECORD
int
memcmp (const void *_s, const void *_s2, size_t capa)
{
  unsigned char *s = (unsigned char *) _s;
  unsigned char *s2 = (unsigned char *) _s2;

  long i;
  rec *item = get_item();
  int latency = cache_probe (s);
  int latency2 = cache_probe (s2);

  for (i = 0; i < capa && s[i] == s2[i]; i++)
    ;

  item->cache_latency = latency;
  item->cache_latency2 = latency2;

  item->s = (uint64_t) s;
  item->s2 = (uint64_t) s2;

  item->capa = capa;
  item->n = i;
  item->success = (s[i] != s2[i]);
  return i == capa ? 0 : s[i] - s2[i];
}
#endif
