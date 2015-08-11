#ifdef WANT_HEADER


typedef struct
{
  uint64_t s;
  uint64_t n;
  uint64_t c;
  uint64_t last_matches;
  uint64_t success;
  uint64_t cache_latency;
} rec;

# define REPLAY_SETUP \
  int ii;\
  char *tmp = malloc (2000000); \
  memset (tmp, 1, 2000000); \
  tmp = tmp + 4096; \
  tmp = tmp - ((uint64_t) tmp) % 4096; 


# ifdef SHOW
#  define REPLAY_ITERATION(rec) printf ("align %8i size %8i character %c(%3i) found: %i latency %4i character pattern 0o%llo\n", rec.s % 64, rec.n, isprint (rec.c) ? rec.c : ' ', rec.c, rec.success, rec.cache_latency, rec.last_matches)
# elif defined SUMMARY


#include "summary_common.h"

# else
#  define REPLAY_ITERATION(rec) \
  if (rec.n < 1000000) \
    { \
      char *start = tmp + (((unsigned long) rec.s) % (1 << 20)); \
      if (rec.c == 1) \
      rec.c = 2; \
      for (ii=0;ii<64;ii++)\
        if (rec.last_matches & (1ULL << ii)) \
          start[rec.n - ii] = rec.c; \
      start[rec.n] = 0; \
      ret += (uintptr_t) strchr (start, rec.c); \
      start[rec.n] = 1; \
      for (ii=0;ii<64;ii++)\
        if (rec.last_matches & (1ULL << ii)) \
          start[rec.n - ii] = 1; \
    }
# endif

#endif

#ifdef WANT_SUMMARY
#define NO_SUCCESS
#include "summary_common.h"
#endif

#ifdef  SUMMARY_SETUP
  double matches = 0; 
#endif
#ifdef SUMMARY_ITERATION
  for (ii=0;ii<64;ii++) 
  if (r.last_matches & (1ULL << i)) 
    matches++;
#endif
#ifdef SUMMARY_PRINT
  printf ("average c matches %7.3lf\n", matches /total_size);
#endif



#ifdef WANT_RECORD
char *
strrchr (const char *_s, int _c)
{
  uint64_t last_matches = 0;
  unsigned char *s = (unsigned char *) _s, c = _c;
  long i;
  rec *item = get_item();
  int latency = cache_probe (s);

  char *last_match = NULL;
  for (i = 0; s[i]; i++)
    if (s[i] == _c)
      {
        last_matches = 2 * last_matches + 1;
        last_match = s + i;
      }
    else
      last_matches *= 2;

  /*handle strrchr (x, 0) */
  if (s[i] == _c)
    last_match = s + i;

      item->cache_latency = latency;
      item->s = (uint64_t) s;
      item->c = c;
      item->n = i;
      item->last_matches = last_matches;
      item->success = (last_match != NULL);
  return last_match;
}
#endif
