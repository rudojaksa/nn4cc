// eval2.c: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// 2nd (advanced) error evaluation log

# include "CONFIG.h"  // fforward
# include "data.h"   // loadpat
# include "aux.h"
# include "logline.h"

# define  AMAE 1  // Average MAE
# define   AED 1  // Average Euclidean Distance
# define  AMSE 1  // Average MSE
# define ARMSE 1  // Average RMSE
# define   AAE 1  // vector of Average Absolute Errors for every output
# define  OUTS 1  // whether outputs will be logged too

// cycle through all outputs
# define fou(i) for(int i=0; i<Nou; i++)

// paths for all datasets
static char *erpath[3];
static char  *oudir[3];

// write header and set paths
void ehdr2(const char *dir, const data_t *data, const int set) {
  newfmt(64);
  erpath[set] = sdup(fmt("%s/er%s.log",dir,data->name));
   oudir[set] = sdup(fmt("%s/ou%s"    ,dir,data->name));
  newdir(oudir[set]);
  FILE *fp = fopenw(erpath[set]);
		 fprintf(fp,"# pat");
  if(AMAE)	 fprintf(fp,"  amae");
  if(AED)	 fprintf(fp," aed");
  if(AMSE)	 fprintf(fp," amse");
  if(ARMSE)	 fprintf(fp," armse ");
  if(AAE) fou(i) fprintf(fp," aae%d",i+1);
		 fprintf(fp,"\n");
  fclose(fp); }

// eval single network on single dataset
double eval2(mlp_t *mlp, const data_t *data, const long long pat, const int set) {
  newfmt(64);
  long P = data->r; // number of patterns

  // open the outputs-log file and write the header
  FILE *fp;
  if(OUTS) {
    fp = fopena(fmt("%s/%09lld.dat",oudir[set],pat));
	     fprintf(fp,"# row");
    if(AMAE) fprintf(fp,"  mae");
    if(AED)  fprintf(fp," ed");
    fou(i)   fprintf(fp,"  yd%d y%d",i+1,i+1);
	     fprintf(fp,"\n"); }

  // aggregated whole-dataset errors
  double  amae	    = 0;
  double  amse	    = 0;
  double armse	    = 0;
  double   aed	    = 0;
  double   aae[Nou] = {0}; // vector of per-output average absolute errors

  // for every pattern
  for(long p=0; p<P; p++) {

    // load inputs and compute network outputs
    loadpat(mlp,data->x,data->c,p);
    fforward(mlp);

    // evaluate every output and aggregate per-pattern sums
    double sae = 0;					// sum of absolute errors
    double sse = 0;					// sum of square errors
    for(int i=OU1; i<=OUn; i++) {			// 
      float err;					// direct error
      float abs;					// absolute error
      if(AMSE||ARMSE||AED||AMAE||AAE) err = yd(i)-y(i);	// just the direct error
      if(AMSE||ARMSE||AED) sse += err*err;		// MSE sum over all outputs
      if(AAE||AMAE) { abs = fabs(err); sae += abs; }	// per-pattern MAE sum over all outputs
      if(AAE) aae[i-OU1] += abs; }			// already here we aggregate over all patterns

    // per-pattern all-outputs errors
    double     mae = 0;
    double     mse = 0;
    double    rmse = 0;
    double      ed = 0;
    if(AMAE)   mae = sae/Nou;
    if(AMSE)   mse = sse/Nou;
    if(ARMSE) rmse = sqrt(sse/Nou);
    if(AED)     ed = sqrt(sse);

    // aggregate over all patterns whole-dataset errors
    if(AMAE)   amae += mae;
    if(AED)     aed += ed;
    if(AMSE)   amse += mse;
    if(ARMSE) armse += rmse;

    // save the outputs line for the current pattern
    if(OUTS) {
      newlogline(line,2+2*Nou);
      if(AMAE)	add1(line,mae/Nou);
      if(AED)	add1(line,ed);
      fou(i)  { adds(line);
	        add2(line,yd(i+OU1),y(i+OU1)); }
      fplogline(fp,p+1,line); }}

  if(OUTS) fclose(fp);

  // compute total average values
  if(AMAE||AAE)	   amae /= P; // AAE needed for reporting the error by return
  if(AED)	    aed /= P;
  if(AMSE)	   amse /= P;
  if(ARMSE)	  armse /= P;
  if(AAE) fou(i) aae[i] /= P;

  // log single line into dedicated errors file
  newlogline(line,4+Nou);
  if(AMAE)	 add1(line,amae);
  if(AED)	 add1(line,aed);
  if(AMSE)	 add1(line,amse);
  if(ARMSE)	 add1(line,armse);
  if(AAE)	 adds(line);
  if(AAE) fou(i) add1(line,aae[i]);
  logline(erpath[set],pat,line);

  // return error for the terminal progress report
  if(AMAE)  return amae;
  if(AED)   return aed;
  if(AMSE)  return amse;
  if(ARMSE) return armse;
  if(AAE)   return amae; }

