#ifdef WANT_HEADER


typedef struct
{
  uint64_t s;
  uint64_t s2;
  uint64_t n;
  uint64_t success;
  uint64_t cache_latency;
  uint64_t cache_latency2;
  uint64_t cmp;
  uint64_t digraph;
  uint64_t trigraph;

  char needle[64];
  char haystack[64];
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
#  define REPLAY_ITERATION(rec) printf ("salign %4i s2align %4i size %8i success: %i latency %4i %4i\n", rec.s % 64,rec.s2 % 64, rec.n,  rec.success, rec.cache_latency, rec.cache_latency2);\
  printf("haystack: %s needle: %s\n",rec.haystack, rec.needle); \
  printf("cmp: %i digraph: %i\n",rec.cmp, rec.digraph);


#elif defined SUMMARY
# include "summary_str_common.h"

# else
#  define REPLAY_ITERATION(rec) \
  if (rec.n < 1000000) \
    { \
      char *start1 = tmp + (((unsigned long) rec.s) % (1 << 20)); \
      char *start2 = tmp2 + (((unsigned long) rec.s2) % (1 << 20)); \
      memcpy(start1, rec.haystack, 64); \
      memcpy(start2, rec.needle, 64); \
      ret += (uintptr_t) strstr (start1, start2); \
    }
# endif

#endif

#ifdef WANT_SUMMARY
#define NO_SUCCESS
#include "summary_str_common.h"


#endif


#ifdef WANT_RECORD
char *
strstr (const char *_haystack, const char *_needle)
{
  unsigned char *haystack = (unsigned char *) _haystack;
  unsigned char *needle = (unsigned char *) _needle;
  char *ret = NULL;
  uint64_t i,j;
  uint64_t cmp = 0, digraph = 0, trigraph = 0;
  rec *item = get_item();
  int latency = cache_probe (haystack);
  int latency2 = cache_probe (needle);

  for (i = 0; haystack[i]; i++)
    {
     
      for (j=0; needle[j] && haystack[i+j]==needle[j];j++);
 
      if (!needle[j])
        {
          ret = (char *) _haystack + i;
          break;
        }
    
      cmp += j+1;

      if (needle[0] == haystack[i] && needle[1] == haystack[i+1])
        digraph++;
      if (needle[0] == haystack[i] && needle[1] == haystack[i+1] && needle[2] == haystack[i+2])
        trigraph++;

    }
  /* Needs terminating zero.  */
  for (i=0;i<63 && haystack[i];i++)
    item->haystack[i] = haystack[i];
  for (i=0;i<63 && haystack[i];i++)
    item->needle[i] = needle[i];

  item->cache_latency = latency;
  item->cache_latency2 = latency2;

  item->s = (uint64_t) haystack;
  item->s2 = (uint64_t) needle;
  item->digraph = digraph;
  item->trigraph = trigraph;
  item->cmp = cmp;
  item->n = i;
  item->success = (ret != NULL);
  return ret;
}
#endif
