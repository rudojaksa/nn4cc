// queue.c: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// queue for multithreaded scheduler

# include "CONFIG.h"
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include "aux.h"

# define MAXTHREADS 32		// number of threads
# define       QLEN 4096	// queue length

static int Threads = 1;		// number of test threads
static int Determ = 0;		// deterministic queue order

static pthread_t threads[MAXTHREADS];	// logger threads
static pthread_mutex_t mutex_queue;	// queue access
static pthread_mutex_t mutex_files;	// write access
static pthread_mutex_t mutex_stderr;	// terminal access

void seize_files()  { pthread_mutex_lock  (&mutex_files); }
void relax_files()  { pthread_mutex_unlock(&mutex_files); }
void seize_stderr() { pthread_mutex_lock  (&mutex_stderr); }
void relax_stderr() { pthread_mutex_unlock(&mutex_stderr); }

static void qseize()  { pthread_mutex_lock  (&mutex_queue); }
static void qrelax()  { pthread_mutex_unlock(&mutex_queue); }

static weights_t **queue; // weights queue
int logstop = 0;	  // whether to stop thread2 = to stop logging

// ----------------------------------------------------------------------------------------------------

// return current queue length
int qlen() {
  int n = 0;
  for(int i=0; i<QLEN; i++) if(queue[i] != 0) n++;
  return n; }

// return 1 if insert succesful
int qinsert(weights_t* const weights) {

  // get free queue position, or -1 if queue is full
  int q = -1;
  for(int i=0;i<QLEN;i++) {
    if(queue[i] == 0) { q=i; break; }}
  if(q == -1) return 0;

  // add request to the queue
  qseize();
  queue[q] = weights;
  qrelax();
  return 1; }

// --------------------------------------------------------------------------------------- THREADS CODE

// private from log.c
extern void logengine(weights_t*);

// logging threads main loop
static void *thread_loop(void *arg) {
  while(1) {				// these threads loop until the explicit stop request
    weights_t *weights = 0;		// 0 means not found

    // select the task to do, lock other searches as well as inserts
    qseize();

    // random tests order
    if(!Determ) {
      if(queue[0]) { weights = queue[0]; queue[0] = 0; }	// try to serve 1st entry 1st
      else {							// 
	int max; for(max=QLEN-1; max>0; max--) if(queue[max]) break; // find the max id in queue
	if(max) {						// if we have at least two entries
	  int rnd = rand() / (RAND_MAX/(max));			// random position in queue
	  for(int i=rnd; i<QLEN; i++) if(queue[i]) {		// search until the end of queue
	      weights = queue[i]; queue[i] = 0;			// save weights and consider task done
	      goto QRELAX; }}					// dont look further
	for(int i=0; i<QLEN; i++) if(queue[i]) {		// one more loop for the case we missed
	    weights = queue[i]; queue[i] = 0;
	    goto QRELAX; }}}

    // deterministic order
    else {
      for(int i=0; i<QLEN; i++) if(queue[i]) {			// search until the end of queue
	weights = queue[i]; queue[i] = 0;			// save weights and consider task done
	break; }}						// dont look further

  QRELAX: // unlock
    qrelax();

    // if no task found
    if(weights == 0) {
      if(logstop != 0) return(0);	// requested stop = end of program
      usleep(100000);			// 100 msec sleep
      continue; }			// go look for next task

    // do the task
    logengine(weights);
    free(weights->w);
    free(weights); }}

// ----------------------------------------------------------------- START & STOP THREADS FOR THE LOG.C

static void (*endfunc)();

// ctrl-c catcher
static void sigend(int signo) {
  logstop = 2;					// request threads to stop themselves
  for(int i=0; i<QLEN; i++) queue[i] = 0;	// clear the queue, we will not wait to finish pending tests
  endfunc();					// call the final sequence, from whoever catched ctrl-c
  exit(130); }					// exit with error

// log.c private: start logging threads
void start_logthreads(void *arg, void (*endfunc0)()) {

  // initialize the queue to zeros
  queue = calloc(QLEN,sizeof(void*));

  // create mutexes
  pthread_mutex_init(&mutex_queue,NULL);
  pthread_mutex_init(&mutex_files,NULL);
  pthread_mutex_init(&mutex_stderr,NULL);

  // set the ctrl-c catcher
  endfunc = endfunc0;		// set the function to be called on the end
  signal(SIGINT,sigend);	// call sigend handler, which will start endfunc

  // start threads
  for(unsigned int i=0; i<Threads; i++) {
    pthread_create(&(threads[i]),NULL,thread_loop,arg); }}

// log.c private: stop logging threads
void stop_logthreads() {
  logstop = 1;					// request all threads to stop
  for(unsigned int i=0; i<Threads; i++) {
    pthread_join(threads[i],NULL); }}		// wait until stopped

// ---------------------------------------------------------------------------------------- QUEUE SETUP

void checkqueue(const int argc, char *argv[]) {

  if(argo("-dt")) Determ = 1;

  Threads = argi("-p",Threads);
  if(Threads > MAXTHREADS) Threads = MAXTHREADS; }

// ----------------------------------------------------------------------------------------------------
