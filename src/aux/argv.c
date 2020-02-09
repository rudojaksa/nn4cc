// argv.c: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// arguments parser

# include <stdlib.h>
# include <stdio.h>
# include "str.h"

// return 1 if switch matched or 0 otherwise, eat all matches
int argo_(const int argc, char *argv[], const char *sw) {
  int ret = 0;
  for(int i=1; i<argc; i++) {
    if(argv[i][0] == 0) continue;
    if(sam(sw,argv[i])) {
      ret = 1;
      argv[i][0] = 0;
      i++; }}
  return ret; }

// return int argument, finish on the first match
int argi_(const int argc, char *argv[], const char *sw, const int dflt) {
  int ret = dflt;
  for(int i=1; i<argc-1; i++) {
    if(argv[i][0] == 0) continue;
    if(sam(sw,argv[i])) {
      ret = atoi(argv[i+1]);
      argv[i][0] = 0;
      argv[i+1][0] = 0;
      i++; break; }}
  return ret; }

// return long long argument, finish on the first match
long long argll_(const int argc, char *argv[], const char* const sw, const long long dflt) {
  long long ret = dflt;
  for(int i=1; i<argc-1; i++) {
    if(argv[i][0] == 0) continue;
    if(sam(sw,argv[i])) {
      ret = atoll(argv[i+1]);
      argv[i][0] = 0;
      argv[i+1][0] = 0;
      i++; break; }}
  return ret; }

// return float argument, convert % values, finish on the first match
float argf_(const int argc, char *argv[], const char *sw, const float dflt) {
  float ret = dflt;
  for(int i=1; i<argc-1; i++) {
    if(argv[i][0] == 0) continue;
    if(sam(sw,argv[i])) {
      int l = len(argv[i+1]) - 1;
      if(argv[i+1][l] == '%') {
	argv[i+1][l] = 0;
	ret = strtof(argv[i+1],0);
	ret /= 100; }
      else {
	ret = strtof(argv[i+1],0); }
      argv[i][0] = 0;
      argv[i+1][0] = 0;
      i++; break; }}
  return ret; }

// return allocated string argument, space-concatenate all matches
char *args_(const int argc, char *argv[], const char *sw, const char *dflt) {
  char ret[1024] = {0};
  for(int i=1; i<argc-1; i++) {
    if(argv[i][0] == 0) continue;
    if(sam(sw,argv[i])) {
      int l = len(ret);
      snprintf(ret+l,1023-l,"%s ",argv[i+1]);
      argv[i][0] = 0;
      argv[i+1][0] = 0;
      i++; }}
  int l = len(ret);
  if(l > 0)	     { ret[l-1] = 0; return sdup(ret); }	// remove the last space and return allocated found string
  else if(dflt == 0) { return 0; }				// return zero
  else		     { return sdup(dflt); }}			// return copy of default string

