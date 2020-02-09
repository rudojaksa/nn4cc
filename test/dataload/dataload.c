# include <string.h>
# include <stdio.h>
# include "../../data.h"
# define COLS 10

int main(int argc, char *argv[]) {

  char *a[2];
  a[1] = strdup("10cols1.dat");
  a[2] = strdup("10cols2.dat");
  if(argc<2) { argv=a; argc=3; }

  files_t *files = checkfiles(argc,argv);
  data_t *data = dataload(files->train,COLS);

  fprintf(stderr,"  cols: %d\n",data->c);
  fprintf(stderr,"  rows: %d\n",data->r);
  fprintf(stderr,"  nums: %d\n",data->N);

  for(int i=0; i<data->N; i++) {
    if(i%COLS==0 && i>0) printf("\n");
    printf("%f ",data->x[i]); }
  printf("\n"); }

