// eval.c: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// common error evaluation code

// TODO: allow -E2 -E3 to be combined, in such case E3 should skip logging amae already provided by E2
//       eval function-pointer then should be changed to aggregation function for several error-types

# include "CONFIG.h"
# include "data.h"
# include "aux.h"
# include "verbose.h"

// function pointers for error-evaluation type selection
  void (*ehdr)(const char*,const data_t*,const int) = 0;
double (*eval)(mlp_t*,const data_t*,const long long,const int) = 0;

// available error-evaluation routines
extern void ehdr1(const char*,const data_t*,const int);
extern void ehdr2(const char*,const data_t*,const int);
extern void ehdr3(const char*,const data_t*,const int);
extern double eval1(mlp_t*,const data_t*,const long long,const int);
extern double eval2(mlp_t*,const data_t*,const long long,const int);
extern double eval3(mlp_t*,const data_t*,const long long,const int);

// set errors mode, after this the ehdr() and eval() point to correct error evaluation functions
void checkmode(int argc, char *argv[]) {
  int Emode=1;		   // 1 = basic AMAE error
  if(argo("-E0")) Emode=0; // none
  if(argo("-E1")) Emode=1; // basic AMAE
  if(argo("-E2")) Emode=2; // advanced incl. outputs
  if(argo("-E3")) Emode=3; // median errors

  if(Emode==1) { ehdr = ehdr1; eval = eval1; }
  if(Emode==2) { ehdr = ehdr2; eval = eval2; }
  if(Emode==3) { ehdr = ehdr3; eval = eval3; }

  char *s;
  if(Emode==0) s = "none";
  if(Emode==1) s = "basic";
  if(Emode==2) s = "advanced";
  if(Emode==3) s = "median";
  journal("error mode","%d %s",Emode,s); }

