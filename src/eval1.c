// eval1.c: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// 1st (basic) error evaluation log

# include "CONFIG.h" // fforward
# include "data.h"  // loadpat
# include "aux.h"
# include "logline.h"

// paths for all datasets
static char *erpath[3];

// write header and return path
void ehdr1(const char *dir, const data_t *data, const int set) {
  dynfmt(64);
  erpath[set] = dfm("%s/er%s.log",dir,data->name);
  FILE *fp = fopenw(erpath[set]);
  fprintf(fp,"# pat  amae\n");
  fclose(fp); }

// eval single network on single dataset
double eval1(mlp_t *mlp, const data_t *data, const long long pat, const int set) {
  long P = data->r; // number of patterns
  double amae = 0;

  // TODO: to run in parallel, instead of mlp_t we would need single set of weights and independet sets of neurons
  // # pragma omp parallel for private(i) reduction(+:mae) schedule(static)
  for(long p=0; p<P; p++) {
    loadpat(mlp,data->x,data->c,p);
    fforward(mlp);
    for(int i=OU1; i<=OUn; i++) amae += fabs(yd(i)-y(i)); }

  amae /= Nou * P;

  // log single line into dedicated errors file
  newlogline(line,1);
  add1(line,amae);
  logline(erpath[set],pat,line);

  // return error for the terminal progress report
  return amae; }

