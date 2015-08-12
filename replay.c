#define _GNU_SOURCE
#include <ctype.h>
#include "common.h"
#ifdef SUMMARY
#define WANT_SUMMARY
#endif
int ext = 0;
int ret;
#ifdef BENCH
# include <time.h>
#endif
#include <string.h>
#undef strchr

#ifndef STRCHR
# define STRCHR strchr
#endif

char *_uniq[10000];
int uniq(void *name)
{
  int i=0,j=0;
  while (_uniq[i])
    {
      if (!strcmp(name,_uniq[i]))
        return 0;
      i++;
    }
  _uniq[i] = strdup(name);
  return 1;
}

int
main (int argc, char **argv)
{
  long i, k;
  rec *buf;
  char *fname;
  int unique = 0;
  long total = 1;
  int totalc =1;
  if (argc > 1)
    total = 0;
     
  if (argv[1] && argv[1][0]=='-' && argv[1][1]=='u')
    unique = 1;

  fname = FNAME;
#if 0
  if (argv[1])
    fname = argv[1];
#endif
  FILE *fi = fopen (fname, "r");
  header *h = malloc (sizeof (header));
  REPLAY_SETUP


#ifdef BENCH
  struct timespec start, end;
  clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &start);
#endif
  long total_size;
  while ((total && totalc--) || (!total && !feof (fi)))
    {
      total_size = 0;
#ifdef SUMMARY
#define SUMMARY_SETUP
#define AVG(i, exp, p) double avg##i = 0;
#define COUNTER(i, exp, p) double cnt##i = 0;
#include FN
#undef SUMMARY_SETUP
#endif
      if (!total)
        totalc = 1;
      while ((!total && totalc--) || (total && !feof (fi)))
        {
          fread (h, sizeof (header), 1, fi);
          buf = malloc (h->size * sizeof (rec));
          fread (buf, h->size * sizeof (rec), 1, fi);
          if (!unique || (h->size > 50 && uniq(h->binary_name)))
            {
#ifdef SUMMARY
              if (h->size > 50 && !total)
#endif
              printf ("\n\nreplaying %s", h->binary_name);


#ifdef BENCH
	      if (unique)
                clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &start);
	      do
	        {
#endif
                  total_size += h->size;
                  for (i = 0; i < h->size; i++)
  	            {
		      rec r = buf[i];
#ifdef SUMMARY
#define SUMMARY_ITERATION
#undef COUNTER
#undef AVG
#define AVG(i, exp, p) avg##i+= exp;
#define COUNTER(i, exp, p) if (exp) cnt##i++;
#include FN
#undef SUMMARY_ITERATION
#endif

#ifndef SUMMARY
                      REPLAY_ITERATION(buf[i]);
#endif
	            }
#ifdef BENCH
    	         }
               while (unique && total_size < 1000000);

             if (unique)
               {
                 clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &end);
                 printf (" took %lli\n", end.tv_nsec - start.tv_nsec + 1000000000L * (end.tv_sec - start.tv_sec));
               }
   
#endif
            }
          free (buf);
        }
#ifdef SUMMARY
      if (total_size > 50)
        {
          printf("\n\ncalls %lli\n",total_size);
#define SUMMARY_PRINT
#undef COUNTER
#undef AVG
#define AVG(i, exp, p) printf("%s %5.4lf ",p, avg##i / total_size);
#define COUNTER(i, exp, p) printf("%s %5.1lf%% ",p,  100 * cnt##i / total_size);
#include FN
        }
#endif
    }
#ifdef BENCH
  clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &end);
  printf (" took %lli\n", end.tv_nsec - start.tv_nsec + 1000000000L * (end.tv_sec - start.tv_sec));
#endif
  fclose (fi);
}
