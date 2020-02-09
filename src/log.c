// log.c: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// logging interface

# include <limits.h>
# include <pthread.h>
# include <time.h>
# include <stdlib.h>
# include "aux.h"
# include "verbose.h"
# include "sugar.h"
# include "sched.h"
# include "queue.h"
# include "eval.h"
# include "wlog.h"
# include "credit.h"
# include "cups.h"
# include "log.h"

static sch_t *sch; // this log.c is only for monitoring of training according to this single scheduler
static data_t **data; // only these datasets are used here

// ------------------------------------------------------------------------------------------ LOG START
static char phead[1024]; // print header
static char prule[1024]; // print rule

extern sch_t *setsched(data_t**,const int);
extern void start_logthreads(void*,void(*)());

// define scheduler and start logging (run the logthread thread from here)
sch_t *scheduler(data_t **data0) {
  char z[32];
  thousandssep();		// set locale...
  data = data0;			// set the data for logging
  sch = setsched(data,TRAIN);	// set scheduler

  // errors files headers
  for(int set=0; set<3; set++) {
    if(!data[set]->Nf) continue;
    if(ehdr) ehdr(Logdir,data[set],set); }
  whdr(Logdir); // weights file header
  chdr(Logdir); // credits file header

  // progress header
  if(VERBOSE) {
    if(!ehdr) {
      verbose0(""); }
    else {
      phead[0]=0; prule[0]=0;
      snprintf(phead,1024,"\n%*s: ",KLEN,"pat");
      snprintf(prule,1024,"  %.*s",KLEN,"--------------------------------");
      for(int g=0; g<3; g++) {
	if(!data[g]->Nf) continue;
	snprintf(phead+len(phead),1024-len(phead),"%6s  ",data[g]->name);
	snprintf(prule+len(prule),1024-len(prule),"--------"); }
      snprintf(prule+len(prule),1024-len(prule),"-");
      verbose0(phead);
      verbose0(prule); }}

  start_logthreads(log,logend_);
  sch->t0 = gettime();
  sch->c0 = thclock();
  journal("train start","%s",dt2s(z,sch->t0));
  return(sch); }

// -------------------------------------------------------------------------------------------- LOG END
extern void stop_logthreads();

// print footer and the CUPS info line
void logend_() {
  sch->cn = thclock();
  sch->tn = gettime();

  char z[32],z2[32]; newfmt(64);
  stop_logthreads();
  sch->pat--; // decrement pat from nextpat() to be exact

  double secs = sch->cn - sch->c0; // clock based
  cups_t cups; getcups(&cups,secs,Nw,sch->pat);
  double pps = sch->pat / secs;

  journal("train stop","%s",dt2s(z,sch->tn));
  journal("train epochs","%s",f2s(z,epochs(sch)));
  journal("train patterns","%lld",sch->pat);
  journal("cons updated","%.0f",cups.cons);
  journal("train time","%s sec",f2s(z,cups.secs));
  journal("mcups","%.1f",cups.cups/1000000);
  journal("pps","%.0f",pps);

  if(VERBOSE) {
    char buf[32];
    if(ehdr) {
      lastunblink(prule);
      verbose(fmt("pat %'lld",sch->pat),"%ld epochs >> "CG_"%s"CD_,sch->epoch,Logdir); }
    else {
      blink(sch->pat,1,"%s epochs >> "CG_"%s"CD_,f2s(z,epochs(sch),1),Logdir);
      verbose0(""); }
    verbose0("\n%*s  "CK_"%s connections updated in %s"CD_,KLEN,metr(buf,cups.cups,"CUPS"),f2ls(z,cups.cons),td2s(z2,cups.secs));
    verbose0("%*s  "CK_"%s patterns updated in %s"CD_"\n",KLEN,metr(buf,pps,"PPS"),f2ls(z,(float)sch->pat),td2s(z2,cups.secs)); }

  // explicite end to finish all threads
  exit(0); }

// ------------------------------------------------------------------------------------ SCHEDULER BLINK
static double clold = -1; // clock for interim cups measurement
static long long old = 0; // last pat

static double o1p = -1;
static double o1c = -1;
static double o2p = -1;
static double o2c = -1;
static int  colon =  1;

void schblink() {

  // cups & pps
  char cups[32]; cups[0]=0;
  char kpps[32]; kpps[0]=0;
  double clnew = thclock();
  if(clold>=0) {
    double t = clnew - clold;
    long long n = sch->pat - old;
    cups_t cc; getcups(&cc,t,Nw,n);
    double  p = n/t;
    double  c = cc.cups;
    double op = p;
    double oc = c;
    if     (o2p >= 0) p = (p+o1p+o2p)/3;
    else if(o1p >= 0) p = (p+o1p)/2;
    if     (o2c >= 0) c = (c+o1c+o2c)/3;
    else if(o1c >= 0) c = (c+o1c)/2;
    o2p = o1p; o1p = op;
    o2c = o1c; o1c = oc;
    kilo(kpps,p,"PPS");
    mega(cups,c,"CUPS"); }
  clold = clnew;
  old = sch->pat;

  // next
  char next[32]; next[0]=0;
  if(ehdr || Wlog) snprintf(next,32,"next="CD_"%'lld"CK_,sch->nextlog);

  // last
  char last[32]; last[0]=0;
  if(sch->pats < LLONG_MAX) {
    if(ehdr || Wlog) snprintf(last,32,"/%'lld",sch->pats);
    else snprintf(last,32,"last=%'lld",sch->pats); }

  // epoch
  char z[32]; char *epoch0 = ""; char epoch1[32]; epoch1[0]=0; char epoch2[32]; epoch2[0]=0;
  if(*next || *last) epoch0 = " ";
  snprintf(epoch1,32, "epoch="CD_"%ld"CK_,sch->epoch); // always
  if(sch->epochs > 0) snprintf(epoch2,32,"/%s",f2s(z,sch->epochs));

  // blink
  blink(sch->pat,colon,CK_"%s%s%s%s%s %s %s"CD_,next,last,epoch0,epoch1,epoch2,cups,kpps);
  colon = (colon==0?1:0); }

// ------------------------------------------------------------------------------------------- LOG CORE
static float        minrmse[3] = {0,0,0}; // min error yet, over all threads
static long long     maxpat = 0;	  // max pat yet
// TODO: save pat-time file

// main log engine called by queue manager as independed thread to fully evaluate given network (weights)
// TODO: const weights
void logengine(weights_t *weights) {

  // create testing network
  mlp_t *mlp = malloc(sizeof(mlp_t));
  y(0)=-1.0;
  weights_load(mlp,weights);

  // compute errors
  double err[3];
  for(int g=0; g<3; g++) { if(!data[g]->Nf) continue; // group missing in given experiment
    if(eval) err[g] = eval(mlp,data[g],weights->pat,g);
    else     err[g] = 0; }

  // report to terminal
  if(VERBOSE) {
    char ers[1024]; ers[0]=0;
    char *c0 = "";
    char z[32];
    if(eval)
      for(int g=0; g<3; g++) { if(!data[g]->Nf) continue;
	char *c1 = "";
	if(minrmse[g] <=0 ) minrmse[g] = err[g];
	if(err[g] <= minrmse[g]) { minrmse[g] = err[g]; c1=CG_; }
	else if(weights->pat < maxpat) { c1=CK_; c0=CK_; }
	if(weights->pat > maxpat) maxpat = weights->pat;
	char *c2 = ""; if(c1[0]!=0) c2=CD_;
	snprintf(ers+len(ers),1024-len(ers),"%s%.5f%s ",c1,err[g],c2); }
    if(sch->pat >= sch->pats) { // after the training ended
      char epoch1[32]; epoch1[0]=0; char epoch2[32]; epoch2[0]=0;
      snprintf(epoch1,32, "epochs="CD_"%ld"CK_,sch->epoch); // always
      if(sch->epochs > 0) snprintf(epoch2,32,"/%s",f2s(z,sch->epochs));
      blink(sch->pat,1,CK_"%s%s queue=%d"CD_,epoch1,epoch2,qlen()); }
    if(eval) cpatunblink(c0,weights->pat,"%s",ers); }
  
  wlog(weights->pat,weights->w);	// weights log
  credit(weights->pat,mlp);		// credits log, mlp has given pat weights loaded
  
  free(mlp); }

// ----------------------------------------------------------------------------------------------------
