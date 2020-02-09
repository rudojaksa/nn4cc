// mlp.c: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// topolgy independent multilayer perceptron code

# include <stdlib.h>	// rand, srand
# include <sys/timeb.h>	// ftime
# include "CONFIG.h"
# include "aux.h"
# include "verbose.h"

// ----------------------------------------------------------------------------- WEIGHTS INITIALIZATION

// random weights init
mlp_t *mlp_rndinit(const float min, const float max, const int seed) {

  struct timeb t; ftime(&t);
  long tm=t.time*1000+t.millitm;
  if(seed == TIMESEED) { srand(tm); } // time-seed random generator 
  else		       { srand(seed); }

  // here we allocate the network
  char z[32];
  mlp_t *mlp = malloc(sizeof(mlp_t));
  journal("weights mem","%.0f kB",sizeof(mlp->w)/1000.0);
  if(!mlp) serror("Cannot allocate network (%s)",f2ms(z,sizeof(mlp_t),"B"));

  y(0)=-1.0; // the input for bias

  for(int i=0; i<Nx; i++)
    for(int j=0; j<Nx; j++)
      w(i,j) = rand() / (RAND_MAX/(max-min)) + min;

  return(mlp); }

// -------------------------------------------------------------------------------------------- LOADPAT

// load row-th pattern form cols-columns data into network
void loadpat(mlp_t *mlp, const float * const data, const int cols, const long row) {
  int ro = row * cols; // row-offset = which-row * no-of-columns
  for(int i=IN1; i<=INn; i++)  y(i) = data[ro + i-IN1];
  for(int i=OU1; i<=OUn; i++) yd(i) = data[ro + i-OU1 + Nin]; } // inputs are 1st columns in datafile

// ----------------------------------------------------------------------------------------------------
