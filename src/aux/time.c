// time.c: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// date and time

# include <time.h>
# include <sys/timeb.h>
# include <stdlib.h>
# include <stdio.h>
# include "fmt.h"

// --------------------------------------------------------------------------

// return current time in seconds
double gettime() {
  struct timeb tb;
  ftime(&tb);
  double sec = tb.time + tb.millitm / 1000.0;
  return sec; }

// get the current thread time in seconds
double thclock() {
  struct timespec ts;
  clock_gettime(CLOCK_THREAD_CPUTIME_ID,&ts);
  double sec = ts.tv_sec + ts.tv_nsec / 1000000000.0;
  return sec; }

// ------------------------------------------------- LINUX SPECIFIC THIDCLOCK

// get the specific thread time in seconds
// static clockid_t clid; // main thread clock id
// pthread_getcpuclockid(pthread_self(),&clid); // obtain clid for current thread
double thidclock(const clockid_t clid) {
  struct timespec ts;
  clock_gettime(clid,&ts);
  double sec = ts.tv_sec + ts.tv_nsec / 1000000000.0;
  return sec; }

// --------------------------------------------------------------------------

// date.time to string: 20190530.075901 ISO8601+; current time if epoch=0
char *dt2s(char *buf, const double epoch) {
  if(!buf) buf = malloc(32*sizeof(char)); // allocate if needed
  time_t t; if(epoch) t = epoch; else t = time(0);
  struct tm *tm = localtime(&t);
  strftime(buf,16,"%Y%m%d.%H%M%S",tm);
  return buf; }

// time difference to string
char *td2s(char *buf, const double dif) { char z[32];
  if(!buf) buf = malloc(32*sizeof(char)); // allocate if needed
  if(dif > 31536000)	snprintf(buf,32,"%s years",f2s(z,dif/31536000,1));
  else if(dif > 86400)	snprintf(buf,32,"%s days", f2s(z,dif/86400,1));
  else if(dif > 3600)	snprintf(buf,32,"%s hr",   f2s(z,dif/3600,1));
  else if(dif > 60)	snprintf(buf,32,"%s min",  f2s(z,dif/60,1));
  else			snprintf(buf,32,"%s sec",  f2s(z,dif,1));
  return buf; }

// --------------------------------------------------------------------------
