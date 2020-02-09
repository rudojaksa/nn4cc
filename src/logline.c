// logline.c: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// write log-line to file

# include <stdarg.h>
# include <stdlib.h>
# include "aux.h"
# include "queue.h"
# include "logline.h"

// -------------------------------------------------------------------------------- ADD NUMBERS TO LINE

// add N numbers into array line of the length Nline, or begin with these numbers
void add_(float *line, int *Nline, const int Mline, int *Xline, const int begin, const int N, ...) {

  // reset extra spaces, and reset pointer to the begin
  if(begin) {
    for(int i=0; i<Mline; i++) Xline[i]=0;
    *Nline = 0; }

  // add all N numbers
  va_list va; va_start(va,N);
  for(int i=0; i<N; i++) {
    line[(*Nline)++] = (float)va_arg(va,double); }
  va_end(va); }

// ----------------------------------------------------------------------------------------- WRITE LINE

// write to already opened file without the pat column
void fplogline_(const float *line, const int Nline, const int Mline, int *Xline, const long long pat, FILE *fp) {
  fprintf(fp,"%lld ",pat);
  for(int i=0; i<Nline; i++) {
    if(Xline[i]) fprintf(fp," ");
    fprintf(fp," %f",line[i]); }
  fprintf(fp,"\n"); }

// lock file access to this tread and open-append-close single line with 1st column pat
void logline_(const float *line, const int Nline, const int Mline, int *Xline, const long long pat, const char *path) {
  seize_files();
  FILE *fp = fopena(path);
  fplogline_(line,Nline,Mline,Xline,pat,fp);
  fclose(fp);
  relax_files(); }

// --------------------------------------------------------------------------------------- WRITE HEADER

void writehead(const char *path, const char *hdr) {
  seize_files();
  FILE *fp = fopena(path);
  fprintf(fp,"%s\n",hdr);
  fclose(fp);
  relax_files(); }

// ----------------------------------------------------------------------------------------------------
