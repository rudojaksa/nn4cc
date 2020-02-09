// credit.c: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// credit logger code

# include <stdio.h>
# include "CONFIG.h"
# include "logline.h"
# include "aux.h"

static int Credit = 0; // whether to log credits (default none)

// set Weights logger
void checkcredit(int argc, char *argv[]) {
  if(argo("-C1")) Credit=1; } // simple input weights aggregates

// paths for all datasets
static char *cpath;

// set path and write header
void chdr(const char* const dir) {
  newfmt(64);
  if(!Credit) return;
  cpath = fmt("%s/credits.log",dir);
  FILE *fp = fopenw(cpath);
  fprintf(fp,"# pat");
  forblk(IN,i) fprintf(fp," in%d",i);
  fprintf(fp,"\n");
  fclose(fp); }

// eval single network on single dataset
void credit(const long long pat, const mlp_t* const mlp) {
  if(!Credit) return;
  newlogline(line,Nin);

  // weights IN->H1
  forblk(IN,j) {
    float in = 0;
    forblk(H1,i) {
      in += fabs(w(i,j)); }
    add1(line,in); }

  logline(cpath,pat,line); }

