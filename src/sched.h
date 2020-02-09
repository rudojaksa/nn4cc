// sched.c: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// logarithmic scheduler and multithreading support for errors logging

# ifndef SCHED_H
# define SCHED_H

# include "CONFIG.h"	// mlp_t
# include "data.h"	// data_t

// training indexes
typedef struct sch_s {
  long long pats;	// number of patterns to learn, the stop condition
  long long pat;	// current global pattern
  long long next;	// next scheduled stop pat, to blink the progress
  long long nextlog;	// next scheduled measurement pat, to log errors
  double nextlogf;	// precious next computed pat to stop, origin of 'next'
  long epoch;		// current epoch
  float epochs;		// number of epochs to learn
  int row;		// current data row
  int rows;		// number of training rows
  double t0,tn;		// times of start/end of training
  double c0,cn;		// clocks of start/end of training
} sch_t;

// return decimal number of epochs done
// inline double epochs(const sch_t* const sch) { return sch->epoch-1 + (double)(sch->row-1)/sch->rows; }
extern double epochs(const sch_t* const);

// # define NOLOG // shut-off all logging
# if defined NOLOG && !defined SCHED_C
  # define   lognow(mlp,sch)
  # define schedule(mlp,sch)
# else
  extern void  lognow(const mlp_t* const,sch_t*);
  extern int   logsch(const mlp_t* const,sch_t*);
//inline int schedule(const mlp_t* const mlp, sch_t *sch) { if(sch->pat >= sch->next) return logsch(mlp,sch); return 0; }
# define schedule(mlp,sch) { if(sch->pat >= sch->next) logsch(mlp,sch); }
# endif

extern int nextpat(mlp_t*,data_t**,sch_t*);
extern void checksched(int,char**);

# endif
