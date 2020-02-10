# include "nn4cc.h"

int main(int argc, char *argv[]) {
  checkargs(argc,argv);

  file_t **files = checkfiles(argc,argv);	// define data files
  data_t **data = dataload(files,Nin+Nou);	// load data from files
  mlp_t *mlp = mlp_rndinit(-0.1,0.1,TIMESEED);	// initialize the network

  sch_t *sch = scheduler(data);			// prepare the log scheduler
  lognow(mlp,sch);				// log 0-th step errors before the training

  while(nextpat(mlp,data,sch)) {		// load next pattern from data to network
    backprop(mlp,Gamma);			// train the network
    schedule(mlp,sch); }			// scheduled error progress log
  logend(sch); }				// print the footer and statistics

