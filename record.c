
#define _GNU_SOURCE
#include <sys/types.h>
#include <unistd.h>

#include "common.h"
#include <string.h>
static void *libm_handle;
#undef strchr
#undef strchrnul

static header h[20000000];

static double (*fn)(double);


static rec *get_item()
{
  /* TODO support resizing, problem is that first call could happen before constructor gets called. */
  if (h->size == 999999)
   return h->items + 999999;
  int idx = __sync_add_and_fetch (&(h->size), 1);
  return h->items + idx;
}

static void
get_binary_name (char *name)
{
  char fname[1000];
  int i;
  sprintf (fname, "/proc/%i/cmdline", getpid ());
  FILE *f = fopen (fname, "r");
  for (i = 0; i < 48; i++)
    name[i] = 0;
  fgets (name, 48, f);
  name[47] = 0;
  fclose (f);
}


/*
   A cache probe determines where in cache hiearchy 
   is cache line pointed by p. 
   It returns cycles needed to fetch p.
 */


static uint64_t __attribute__ ((noinline))
cache_probe2 (char *p)
{
#ifdef __x86_64__
  uint64_t start;
  uint32_t lo, hi;
  p = p - (((uintptr_t) p) % 64);

  __asm__ __volatile__ (
    "        xorl %%eax,%%eax \n"
    "        cpuid"      // serialize
    ::: "%rax", "%rbx", "%rcx", "%rdx");
/* We cannot use "=A", since this would use %rax on x86_64 and return only the lower 32bits of the TSC */

  __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
  start = (uint64_t) hi << 32 | lo;

  uint64_t r = *((uint64_t *) p);

  __asm__ __volatile__ (
    "        xorl %%eax,%%eax \n"
    "        cpuid"      // serialize
    ::: "%rax", "%rbx", "%rcx", "%rdx");
  __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
  return ((uint64_t) hi << 32 | lo) - start + r % 2;
#endif
  return 0;
}

static uint64_t __attribute__ ((noinline))
cache_probe (char *p)
{
#ifdef __x86_64__
  __asm__ __volatile__ (
    "        xorl %%eax,%%eax \n"
    "        cpuid"      // serialize
    ::: "%rax", "%rbx", "%rcx", "%rdx");
#endif

  /* Remove overhead of serializing. */
  /* TODO standardize as differences change based on frequency.  */
  int r = cache_probe2(p) - cache_probe2(p);
  return r > 0 ? r : 0;
}




void
lock_file (FILE *fi)
{
  struct flock fl;
  int fd = fileno (fi);
  fl.l_type = F_WRLCK;
  fl.l_whence = SEEK_SET;
  fl.l_start = 0;
  fl.l_len = 0;
  fl.l_pid = getpid ();
  fcntl (fd, F_SETLKW, &fl);
}

__attribute__ ((destructor)) static void
__end_profile ()
{
  int i, j;
  get_binary_name (h->binary_name);
  FILE *fi = fopen (FNAME, "a");
  if (fi)
    {
		lock_file(fi);
      fwrite (h, sizeof (header) + h->size * sizeof (rec), 1, fi);
      fclose (fi);
    }
}

#define WANT_RECORD
#include FN
