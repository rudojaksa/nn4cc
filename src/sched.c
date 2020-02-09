// sched.c: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// logarithmic scheduler and multithreading support for errors logging

# define SCHED_C
# include <limits.h>
# include <time.h>
# include <stdlib.h>
# include <stdio.h>
# include "aux.h"
# include "sugar.h"
# include "cups.h"
# include "verbose.h"
# include "queue.h"
# include "sched.h"

static float Logstep = 1.3; // multiplier for current pat to get next one

// -------------------------------------------------------------------------------------- IMMEDIATE LOG
//static int firstlog = 0; // whether first log happened

// immediate log
void lognow(const mlp_t *const mlp, sch_t *sch) {
  
  // sample the weights
  weights_t *weights = weights(mlp);
  weights->pat = sch->pat;

  // TODO: this corrects t0 from scheduler(), but expects lognow() on pat 0 just before training
  // if(!firstlog) { sch->c0 = thclock(clid); sch->t0 = gettime(); firstlog = 1; }

  // insert request to the queue
  if(!qinsert(weights)) patunblink(sch->pat,"log queue full, skip eval..."); }

// -------------------------------------------------------------------------------------- SCHEDULED LOG
# define MSEC 90 // period in msecs for progress update

extern void schblink();
extern int logstop;					// queue.c
static double clock1 = -1; static long long pat1 = 0;	// previous stoptime, for measuring blink period
static double clock2 = -1;				// current stoptime
static long patstep = 0;				// progress-update pat-count

// log and schedule the next stop
// return: 0=break-by-the-supervisor 1=everything-done 2=log-not-yet-needed
// called by schedule() from sched.h, which should by called directly from the application
int logsch(const mlp_t *const mlp, sch_t *sch) {

  // ### 1st: blink ###
  if(!Noprogr) schblink();

  // ### 2nd: stop ###
  if(logstop > 1) exit(0); // exit the main thread if logstop initiated by ctrl-c
  if(logstop > 0) { sch->pat = sch->pats + 1; return 0; } // logend() called from the main program
  
  // ### 3rd: next blink scheduling and skiplog ###

  // patstep not yet deremined, so measure it
  if(!patstep) {
    clock2 = thclock();							// get current time
    if(clock1>=0) {							// if period was >MSEC/10 save pat-count (10% precision)
      double secs = clock2 - clock1;					// compute time difference
      if(secs*10000 > MSEC) {						// compare it to MSEC/10 in miliseconds
	patstep = (sch->pat - pat1) * 10;				// how many pats corresponds to MSEC (*10 because we had MSEC/10)
	journal("pat step","%d",patstep);
	if(Seconds > 0) {						// if requested timed training end, estemate the last pat
	  long long pats = patstep/secs * Seconds / 10;			// estiamate the end
	  if(pats < sch->pats) {					// if it looks earlier then otherwise chosen end, use this one
	    sch->pats = pats;						// use it
	    journal("stop pat","%lld",sch->pats); }}}}			// report it
    clock1 = clock2; pat1 = sch->pat;					// remember time and pat for the next measurement
    sch->next = sch->nextlog; }						// patstep not known: so just stop on the next log ...TODO: nextlog might be to sparse

  // patstep known, so set next blink
  else {
    sch->next += patstep;						// patstep known: so use it
    if(sch->next < sch->nextlog) return 2;				// no log needed, so lets go for next blink
    else sch->next = sch->nextlog; }					// log will happen erlier then the blink would

  // ### 4th: log ###
  lognow(mlp,sch);

  // ### 5th: look for next log ###
  long long nextlog;
  do {
    sch->nextlogf *= Logstep;			// advance by Logstep times
    if(sch->nextlogf < 1) sch->nextlogf = 1;	// escape from zero
    nextlog = (long long)ceil(sch->nextlogf); }	// round
  while(nextlog <= sch->nextlog);		// advance again until meaningful
  sch->nextlog = nextlog;
  if(sch->pats > 0 && sch->nextlog > sch->pats) sch->nextlog = sch->pats;

  return 1; }

// -------------------------------------------------------------------------------------- SET SCHEDULER

// return index initialized for given data and number of epochs
sch_t *setsched(data_t **data, const int role) {
  sch_t *sch = malloc(sizeof(sch_t));

  // role
  int r = 0; // default is zero
  if(role==TEST1) r=1;
  if(role==TEST2) r=2;

  // rows, epochs
  sch->row = 0;		 // the 1st row is zero
  sch->rows = data[r]->r;
  sch->epoch = 1;	 // the 1st epoch is 1
  sch->epochs = Epochs;

  // stop
  sch->pats = LLONG_MAX;
  if(Epochs > 0) sch->pats = ceil(Epochs * data[r]->r); // finish = epochs * rows
  if(Pats > 0 && Pats < sch->pats) sch->pats = Pats;	// Pats < Epochs
  if(sch->pats < LLONG_MAX) journal("stop pat","%lld",sch->pats);

  // scheduler
  sch->pat = 0;			// 0 = before traning start
  sch->next = 0;		// next blink pat
  sch->nextlog = 0;		// next log pat
  sch->nextlogf = 0;		// next log precise sub-pat

  return sch; }

// --------------------------------------------------------------------------------------------- EPOCHS

// return decimal number of epochs done
double epochs(const sch_t* const sch) { return sch->epoch-1 + (double)(sch->row-1)/sch->rows; }

// -------------------------------------------------------------------------------------------- NEXTPAT

// TODO! first check then load

// load next pattern to the network, or return zero when training should end
int nextpat(mlp_t *mlp, data_t **data, sch_t *sch) {

  loadpat(mlp, data[0]->x, data[0]->c, sch->row);
  sch->row++; if(sch->row >= data[0]->r) { sch->row=0; sch->epoch++; }
  sch->pat++; if(sch->pat > sch->pats) return 0; // final pat was processed = training finished

  return 1; }

// ------------------------------------------------------------------------------------ SCHEDULER SETUP

void checksched(int argc, char *argv[]) {
  char z[32];

  // logstep
  Logstep = argf("-s",Logstep);
  if(Logstep <= 1) Logstep += 1; // allow to input relative "10%" step

  // xverbose("logstep",f2s(z,Logstep));
  journal("logstep",f2s(z,Logstep)); }

// ----------------------------------------------------------------------------------------------------
