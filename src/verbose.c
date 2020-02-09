// verbose.c: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// verbose messaging support

# include <stdarg.h>
# include <stdlib.h>
# include <errno.h>
# include <string.h>	// strerror
# include <stdio.h>
# include "global.h"
# include "aux.h"
# include "files.h"
# include "sugar.h"
# include "queue.h"
# include "verbose.h"

int Verbose = 0;

// --------------------------------------------------------------------------------------------- ERRORS

// universal error engine
static void errorer(sys,fmt,va)
  const int sys;	// whether to print system error message
  const char *fmt;	// printf-like format
  va_list va; {		// printf-like args
  fprintf(stderr,"\n"CR_);
  vfprintf(stderr,fmt,va);
  if(sys) fprintf(stderr,": %s",strerror(errno));
  fprintf(stderr,CD_"\n\n"); }

// error without system errno
void error(const char *fmt, ...) {
  va_list va; va_start(va,fmt);
  errorer(0,fmt,va);
  va_end(va);
  exit(1); }

// with system errno
void serror(const char *fmt, ...) {
  va_list va; va_start(va,fmt);
  errorer(1,fmt,va);
  va_end(va);
  exit(1); }

// -------------------------------------------------------------------------------------------- JOURNAL

// journal engine
static void journaler(const char *key, const char *fmt, va_list va) {
  FILE *fp = fopena(Journal);		// open/close for every message
  if(key) fprintf(fp,"%s: ",key);	// add key prefix
  vfprintf(fp,fmt,va);			// print new string
  fprintf(fp,"\n");			// end of message line
  fclose(fp); }

// write journal line
void journal(const char *key, const char *fmt, ...) {
  va_list va; va_start(va,fmt);
  journaler(key,fmt,va);
  va_end(va); }

// -------------------------------------------------------------------------------------------- VERBOSE

// universal verbose engine
static void verboser(nl,key,fmt,va)
  const int nl;		// whether to print newline
  const char *key;	// string as prefix, or nothing if 0
  const char *fmt;	// printf-like format
  va_list va; {		// printf-like args
  seize_stderr();
  if(key) fprintf(stderr,"%*s: ",KLEN,key);		// add key prefix
  vfprintf(stderr,fmt,va);				// print new string
  if(nl) fprintf(stderr,"\n");				// this is end of msg
  relax_stderr(); }

// extra verbose message, with the preceding key string
void xverbose(const char *key, const char *fmt, ...) {
  if(!Verbose) return;
  va_list va; va_start(va,fmt);
  verboser(1,key,fmt,va);
  va_end(va); }

// unconditional verbose
void verbose(const char *key, const char *fmt, ...) {
  va_list va; va_start(va,fmt);
  verboser(1,key,fmt,va);
  va_end(va); }

// message without key prefix
void verbose0(const char *fmt, ...) {
  va_list va; va_start(va,fmt);
  verboser(1,0,fmt,va);
  va_end(va); }

// verbose single-line 2-part sequence: part 1 
void verbose1(const char *key, const char *fmt, ...) { // message
  va_list va; va_start(va,fmt);
  verboser(0,key,fmt,va);
  va_end(va); }
// part 2
void verbose2(const char *fmt, ...) {
  va_list va; va_start(va,fmt);
  verboser(1,0,fmt,va);
  va_end(va); }

// ---------------------------------------------------------------------------------------------- BLINK
static char blbuf[1024] = {0};	// last blinked message
static  int bllen = 0;		// its un-escaped length

// add space to the s to cover the the tail of previous blink
static void addspace(char *s, const int l) {
  int len = bllen - esclen(s);
  if(len <= 0) return;
  int i=0;
  while(i < len) {
    s[l+i] = ' ';
    if(l+i == 1024) break;
    i++; }
  s[l+i]=0; }

// blink core engine
static void blinker(stay,end,colon,c1,pat,key,fmt,va)
  const int stay;	// whether the string will stay, or just blink
  const int end;	// whether this is the last string in the blink sequence
  const int colon;	// 1=':' 0=' '
  const char *c1;	// color or ""
  const long long pat;	// pat number as a prefix, or nothing if pat<0
  const char *key;	// string as prefix, or nothing if 0
  const char *fmt;	// printf-like format
  va_list va; {		// printf-like args
  char s[1024];
  int l = 0;
  char *c2 = ""; if(c1[0]!=0) c2=CD_;					// automaticaly set color reset when needed
  char *c = (colon?":":CR_":");
  if(pat >= 0) l += snprintf(s,1024,"%s%' *lld%s%s ",c1,KLEN,pat,c,c2);	// add pat prefix
  if(key)      l += snprintf(s,1024,"%s%*s%s%s ",c1,KLEN,key,c,c2);	// add key prefix
  addspace(s,l+vsnprintf(s+l,1024-l,fmt,va));				// add blink-removing space
  if(!stay) { snprintf(blbuf,1024,"%s",s); bllen = esclen(blbuf); }	// save blbuf, because this is blink
                     seize_stderr();
                     fprintf(stderr,"\r%s",s);		// print new string
  if(stay) { if(end) fprintf(stderr,"\n");		// don't repeat the blink, this is the end
             else    fprintf(stderr,"\n%s",blbuf); }	// reprint last blink
                     relax_stderr(); }

// blink-print temporary progress info
void blink(const long long pat, const int colon, const char *fmt, ...) { 
  va_list va; va_start(va,fmt);
  blinker(0,0,colon,"",pat,0,fmt,va);
  va_end(va); }

// print permanent line
void unblink(const char *fmt, ...) {
  va_list va; va_start(va,fmt);
  blinker(1,0,1,"",(long long)-1,0,fmt,va);
  va_end(va); }

// print permanent line, last in the sequence, without following blink
void lastunblink(const char *fmt, ...) {
  va_list va; va_start(va,fmt);
  blinker(1,1,1,"",(long long)-1,0,fmt,va);
  va_end(va); }

// print permanent line, with the preceding pat number
void patunblink(const long long pat, const char *fmt, ...) {
  va_list va; va_start(va,fmt);
  blinker(1,0,1,"",pat,0,fmt,va);
  va_end(va); }
// color patunblink
void cpatunblink(const char *c1, const long long pat, const char *fmt, ...) {
  va_list va; va_start(va,fmt);
  blinker(1,0,1,c1,pat,0,fmt,va);
  va_end(va); }

// print permanent line, with the preceding key string
void keyunblink(const char *key, const char *fmt, ...) {
  va_list va; va_start(va,fmt);
  blinker(1,0,1,"",(long long)-1,key,fmt,va);
  va_end(va); }

// ----------------------------------------------------------------------------------------------------
