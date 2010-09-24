/* -*- mode: C; mode: folding; fill-column: 70; -*- */
#include "compat.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <unistd.h>
#include <time.h>

#if defined(CLOCK_MONOTONIC)
#define TICTOC_CLOCK CLOCK_MONOTONIC
#define TICTOC_CLOCK_NAME "CLOCK_MONOTONIC"
#elif defined(CLOCK_REALTIME)
#define TICTOC_CLOCK CLOCK_REALTIME
#define TICTOC_CLOCK_NAME "CLOCK_REALTIME"
#else
#error "Failed to find a timing clock."
#endif

#if defined(__MTA__)
long tic_ts;
#elif defined(__MacOSX__)
static AbsoluteTime tic_ts;
#else
static struct timespec tic_ts;
#endif

void
tic (void)
{
#if defined(__MTA__)
  tic_ts = mta_get_clock (0);
#elif defined(__MacOSX__)
  tic_ts = UpTime ();
#else
  clock_gettime (TICTOC_CLOCK, &tic_ts);
#endif
}

double
toc (void)
{
  double out;
#if defined(__MTA__)
  long ts;
  ts = mta_get_clock (0);
  out = ((double)ts - tic_ts) / mta_clock_freq ();
#elif defined(__MacOSX__)
  AbsoluteTime ts;
  ts = UpTime ();
  out = 1.0e-9 * AbsoluteDeltaToNanoseconds (ts, tic_ts);
#else
  struct timespec ts;
  clock_gettime (TICTOC_CLOCK, &ts);
  out = (ts.tv_nsec - (double)tic_ts.tv_nsec) * 1.0e-9;
  out += (ts.tv_sec - (double)tic_ts.tv_sec);
#endif

  return out;
}
