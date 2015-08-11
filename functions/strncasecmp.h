#ifdef WANT_HEADER
#include <ctype.h>

typedef struct
{
  uint64_t s;
  uint64_t s2;
  uint64_t n;
  uint64_t capa;
  uint64_t success;
  uint64_t cache_latency;
  uint64_t cache_latency2;
  uint64_t case_match;
} rec;

# define REPLAY_SETUP \
  int ii;\
  char *tmp = malloc (2000000); \
  memset (tmp, 'a', 2000000); \
  tmp = tmp + 4096; \
  tmp = tmp - ((uint64_t) tmp) % 4096; \
  char *tmp2 = malloc (2000000); \
  memset (tmp2, 'a', 2000000); \
  tmp2 = tmp2 + 4096; \
  tmp2 = tmp2 - ((uint64_t) tmp2) % 4096; 

# ifdef SHOW
#  define REPLAY_ITERATION(rec) printf ("salign %4i s2align %4i size %8i capacity %8i success: %i latency %4i %4i case mismatches: 0o%llo\n", rec.s % 64,rec.s2 % 64, rec.n, rec.capa,  rec.success, rec.cache_latency, rec.cache_latency2, rec.case_match);

#elif defined SUMMARY

# define USES_CAPA
# define SUMMARY_SETUP2 double mismatch = 0;
# define REPLAY_ITERATION2(rec) for (ii=0;ii<64;ii++) if (rec.case_match & (1ULL << ii)) mismatch += 1;
# define PRINT_REPLAY_SUMMARY2 printf("average case mismatches %7.3lf\n", mismatch / total_size);
# include "summary_cmp_common.h"

# else
#  define REPLAY_ITERATION(rec) \
  if (rec.n < 1000000) \
    { \
      char *start1 = tmp + (((unsigned long) rec.s) % (1 << 20)); \
      char *start2 = tmp2 + (((unsigned long) rec.s2) % (1 << 20)); \
      start1[rec.n] = rec.success ? 2 : 0; \
      for (ii=0;ii<64;ii++)\
        if (rec.case_match & (1ULL << ii)) \
          start1[rec.n - ii] = 'A'; \
    ret += (uintptr_t) strncasecmp (start1, start2,rec.capa); \
      start1[rec.n] = 1; \
    for (ii=0;ii<64;ii++)\
        if (rec.case_match & (1ULL << ii)) \
          start1[rec.n - ii] = 'a'; \
    }
# endif

#endif

#ifdef WANT_SUMMARY
#define NO_SUCCESS
#include "summary_cmp_common.h"
#endif

#ifdef WANT_RECORD
int
strncasecmp (const char *_src, const char *_src2, size_t capa)
{
  unsigned char *src = (unsigned char *) _src;
  unsigned char *src2 = (unsigned char *) _src2;
  uint64_t mask = 0;
  long i;
  rec *item = get_item();
  int latency = cache_probe (src);
  int latency2 = cache_probe (src2);

  for (i = 0; i < capa && src[i] && tolower(src[i]) == tolower(src2[i]); i++)
    if (src[i] != src2[i])
      mask = 2* mask +1;
    else
      mask = 2* mask;

  item->cache_latency = latency;
  item->cache_latency2 = latency2;

  item->s = (uint64_t) src;
  item->s2 = (uint64_t) src2;
  item->capa = capa;
  item->n = i;
  item->case_match = mask;
  item->success = (tolower(src[i]) != tolower(src2[i]));
  if (i==capa)
    return 0;
  return tolower(src[i]) - tolower(src2[i]);
}
#endif
