#ifdef WANT_HEADER


typedef struct
{
  uint64_t s;
  uint64_t n;
  uint64_t c;
  uint64_t success;
  uint64_t cache_latency;
  char accept[64];
} rec;

# define REPLAY_SETUP \
  char *tmp = malloc (2000000); \
  memset (tmp, 1, 2000000); \
  tmp = tmp + 4096; \
  tmp = tmp - ((uint64_t) tmp) % 4096; 



# ifdef SHOW
#  define REPLAY_ITERATION(rec) printf ("align %8i size %8i character %c(%3i) found: %i latency %i accept: '%s'\n", rec.s % 64, rec.n, isprint (rec.c) ? rec.c : ' ', rec.c, rec.success, rec.cache_latency, rec.accept)
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
#include "summary_common.h"
COUNTER (100, strlen(r.accept) <= 0, "strlen(r.accept) <= 0:");
COUNTER (101, strlen(r.accept) <= 4, "strlen(r.accept) <= 4:");
COUNTER (102, strlen(r.accept) <= 8, "strlen(r.accept) <= 8:");
COUNTER (103, strlen(r.accept) <= 16, "strlen(r.accept) <= 16:");
COUNTER (104, strlen(r.accept) <= 24, "strlen(r.accept) <= 24:");
COUNTER (105, strlen(r.accept) <= 32, "strlen(r.accept) <= 32:");
COUNTER (106, strlen(r.accept) <= 48, "strlen(r.accept) <= 48:");
COUNTER (107, strlen(r.accept) <= 64, "strlen(r.accept) <= 64:");


#endif


#ifdef WANT_RECORD

static
int member (char c, char *ac)
{
  while (*ac)
    {
      if (*ac == c)
        return 1;
      ac++;
    }
  return 0;
}
#undef strpbrk
#undef strcspn
char *
strpbrk (const char *_s, const char *_ac)
{
  unsigned char *s = (unsigned char *) _s, *ac = (unsigned char *) _ac;
  long i;
  rec *item = get_item();
  int latency = cache_probe (s);

  for (i = 0; s[i] && !member(s[i], ac); i++)
    ;

      item->cache_latency = latency;
      item->s = (uint64_t) s;
      item->c;
      item->n = i;
      item->success = member(s[i], ac);
  memcpy(item->accept, ac, strlen(ac) < 63 ? strlen(ac) : 63);
  return member(s[i], ac) ? s + i: NULL;
}

size_t strcspn(const char *s, const char *reject)
{
  char *p = strpbrk(s, reject);
  if (p)
    return p - s;
  return strlen(s);
}
#endif
