// eval3.c: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// 3rd (median) error evaluation log

# include <float.h>  // DBL_MAX
# include "CONFIG.h" // fforward
# include "data.h"  // loadpat
# include "aux.h"
# include "logline.h"

// paths for all datasets
static char *erpath[3];

// write header and return path
void ehdr3(const char *dir, const data_t *data, const int set) {
  dynfmt(64);
  erpath[set] = dfm("%s/er%s.log",dir,data->name);
  FILE *fp = fopenw(erpath[set]);
  fprintf(fp,"# pat  amae mmae  t01mae t1mae t10mae t40mae t49mae b49mae b40mae b10mae b1mae b01mae\n");
  fclose(fp); }

// compare for qsort
static int fcmp(const void* const ap, const void* const bp) {
  const float a = *((const float*)ap);
  const float b = *((const float*)bp);
  if     (a<b) return -1;
  else if(a>b) return  1;
               return  0; }

// eval single network on single dataset
double eval3(mlp_t *mlp, const data_t *data, const long long pat, const int set) {
  long P = data->r;			// number of patterns
  double amae = 0;			// whole-dataset average MAE
  float	*mae = calloc(P,sizeof(float));	// all-outputs MAE for every pattern

  for(long p=0; p<P; p++) {
    loadpat(mlp,data->x,data->c,p);
    fforward(mlp);
    float sae = 0; // sum of absolute errors
    for(int i=OU1; i<=OUn; i++) sae += fabs(yd(i)-y(i));
    mae[p] = sae/Nou;
    amae  += sae/Nou; }

  // compute total average MAE
  amae /= P;

  // sort and median-like errors
  qsort(mae,P,sizeof(float),fcmp);
  double mmae	= mae[(int)(P*0.5)];	// median MAE
  double t01mae	= mae[(int)(P*0.001)];	// top 0.1% MAE
  double t1mae	= mae[(int)(P*0.01)];	// top 1% MAE
  double t10mae	= mae[(int)(P*0.10)];	// top 10% MAE
  double t40mae	= mae[(int)(P*0.40)];	// top 40% MAE
  double t49mae	= mae[(int)(P*0.49)];	// top 49% MAE
  double b49mae	= mae[(int)(P*0.51)];	// bottom 49% MAE
  double b40mae	= mae[(int)(P*0.60)];	// bottom 40% MAE
  double b10mae	= mae[(int)(P*0.90)];	// bottom 10% MAE
  double b1mae	= mae[(int)(P*0.99)];	// bottom 1% MAE
  double b01mae	= mae[(int)(P*0.999)];	// bottom 0.1% MAE

  // log single line into dedicated errors file
  newlogline(line,12);
  add2(line,amae,mmae);
  adds(line);
  add5(line,t01mae,t1mae,t10mae,t40mae,t49mae);
  add5(line,b49mae,b40mae,b10mae,b1mae,b01mae);
  logline(erpath[set],pat,line);

  // return error for the terminal progress report
  return amae; }

