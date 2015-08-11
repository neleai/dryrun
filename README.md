# dryrun
A tool for collecting and replaying function traces.

Main aim is to analyze workloads how functions are used.
It allows to record profile trace, then replay it and compute 
arbitrary statistic from collected data.

To analyze function you start by writing file functions/function.h that will be explained later. For many glibc functions a file is already present. 
Once you wrote that file(s) you compile all functions by typing 

make

Then you need to record some data. Simplest way to do that is just write

./record

This starts bash and dryrun will record every command in that shell until you exit it. If you want to do recording for only specific functions you need to 
LD_PRELOAD=path_to_dryrun/bin/record_function.so command

Once you do this recorded data is saved into bin/function.rec file. It could be used in several ways.

A simplest one is to print arguments and other informantion about each call. For this you use

./bin/show_function

To get less raw information call a summary which prints you statistic about that function. Use flag -u to print statistic on per-binary-name basis or -a to print it on per-process basis by

./bin/summary_function

If you want to try a new function implementation compile it as shared library 
Then test these by repeatedly issuing command

LD_PRELOAD=old_implementation.so ./bin/bench_function
LD_PRELOAD=new_implementation.so ./bin/bench_function

Now its time to explain format of functions/function.h file from which we generate these commands.

First you need to create structure how you save data on disk. Currently for code simplicity you need to use fixed size, you need to surround it by #ifdef WANT_HEADER

#ifdef WANT_HEADER

typedef struct
{
  uint64_t foo;
  uint64_t bar;

} rec;

#endif

Then you need to intercept function calls for recording. You do it by redefining function and calling original function once you recorded data like here:

#ifdef WANT_RECORD
#define _GNU_SOURCE
#include <dlfcn.h>
int 
baz (int foo, int bar)
{
  rec *item = get_item();
  item->foo = foo;
  item->bar = bar;
  int (*real_baz) = dlsym (RTLD_NEXT, "baz");
  return real_baz (foo, bar);
}
#endif

Once you recorded these you need to specify how to replay them. Use macro REPLAY_SETUP to initialize common data. Then you need to define REPLAY_ITERATION(rec) to actually call function in benchmark or print it

# ifdef SHOW
#  define REPLAY_ITERATION(rec) printf("baz (%i, %i)\n", rec.foo, rec.bar);
# else
#  define REPLAY_ITERATION(rec) baz(rec.foo, rec.bar);
# endif

For summary easiest way to use it is to use macros to define average and macro to compute percentage of calls where functions is true by:

#ifdef WANT_SUMMARY
AVG(1, r.foo,"\navegarge foo: ");
AVG(2, r.bar,"\navegarge bar: ");
COUNTER(3, r.bar % 42 == 16,"\nProbability of bar % 42 == 16: ");
#endif

If you need to do more complicated computation use following pattern.

#ifdef SUMMARY_SETUP
int foobar = 0;
#endif

#ifdef SUMMARY_ITERATION
  foobar += r.foo + r.bar
#endif

#ifdef SUMMARY_PRINT
   printf ("total foo+bar: %i\n", foobar);
#endif
