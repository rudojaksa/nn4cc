// fs.c: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// filesystem operations

# include <sys/stat.h>
# include <stdarg.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <errno.h>
# include "color.h"
# include "str.h"

static void serror(const char *fmt, ...) {
  va_list va; va_start(va,fmt);
  fprintf(stderr,"\n"CR_);
  vfprintf(stderr,fmt,va);
  fprintf(stderr,": %s"CD_"\n\n",strerror(errno));
  va_end(va);
  exit(1); }

// -----------------------------------------------------------------

// auxiliary float array malloc
float *falloc(const int n) {
  float *p = malloc(n * sizeof(float));
  if(!p) serror("can't allocate array");
  return p; }

// -----------------------------------------------------------------

// fopen-read wrapper
FILE *fopenr(const char *path) {
  FILE *fp = fopen(path,"r");
  if(!fp) serror("can't read-open %s",path);
  return fp; }

// fopen-write wrapper
FILE *fopenw(const char *path) {
  FILE *fp = fopen(path,"w");
  if(!fp) serror("can't write-open %s",path);
  return fp; }

// fopen-append wrapper
FILE *fopena(const char *path) {
  FILE *fp = fopen(path,"a");
  if(!fp) serror("can't append-open %s",path);
  return fp; }

// -----------------------------------------------------------------

// return basename without directory and suffixes
char *basename(const char *path) {
  char *s = sdup(path);
  int l = len(s);
  int i0 = 0;
  for(int i=l-1; i>=0; i--) {
    if(s[i] == '.') s[i] = 0;
    if(s[i] == '/') { i0 = i+1; break; }}
  return sdup(s+i0); }

// return filename without directory (recycle the path string)
char *fname(char *path) {
  int l = len(path);
  int i0 = 0;
  for(int i=l-1; i>=0; i--) {
    if(path[i] == '/') { i0 = i+1; break; }}
  return path+i0; }

// -----------------------------------------------------------------

// check whether file/directory exists
int exists(const char *path) {
  struct stat st;
  return stat(path,&st)==0; }

// return 1 if file is empty or nonexistent
int empty(const char *path) {
  struct stat st;
  if(stat(path,&st) != 0) return 1;
  if(st.st_size < 1) return 1;
  return 0; }

// -----------------------------------------------------------------

// recursively create direcotry
// not optimal: newdir("1/../2") will unnecesary create 1/ too
void newdir(const char *dir) {
  char *sub = sdup(dir);
  for(int i=len(dir)-1; i>=0; i--) {
    if(sub[i] == '/') { sub[i] = 0; break; }
    if(i == 0) sub[i] = 0; }
  if(sub[0] == 0) { mkdir(dir,0755); }
  else {
    struct stat st;
    if(stat(sub,&st)!=0) newdir(sub);
    if(stat(dir,&st)!=0) mkdir(dir,0755); }}

// -----------------------------------------------------------------
