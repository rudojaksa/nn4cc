// wlog.c: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// weights logger code

# include <stdio.h>
# include "CONFIG.h"
# include "logline.h"
# include "aux.h"

int Wlog = 1; // whether to log weights (default yes)

// set Weights logger
void checkwlog(int argc, char *argv[]) {
  if(argo("-W0")) Wlog=0; } // none

// paths for all datasets
static char *wpath;

// write header and return path
void whdr(const char* const dir) {
  if(!Wlog) return;
  dynfmt(64);
  wpath = dfm("%s/weights.log",dir);
  FILE *fp = fopenw(wpath);
  fprintf(fp,"# pat");
  for(int i=0; i<Nw; i++) fprintf(fp," w%d",i+1);
  fprintf(fp,"\n");
  fclose(fp); }

// local arrays bigger than 100000 are problem, thus we will use malloc
# if Nw > 1000000
#   undef  newlogline
#   define newlogline(line,length) dynlogline(line,length)
# else
#   undef  freelogline
#   define freelogline(line)
# endif

// eval single network on single dataset
void wlog(const long long pat, const float* const weights) {
  if(!Wlog) return;
  newlogline(line,Nw);
  for(int i=0; i<Nw; i++) add1(line,weights[i]);
  logline(wpath,pat,line);
  freelogline(line); }

