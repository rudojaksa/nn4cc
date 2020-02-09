// str.c: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// string functions

# include <stdlib.h>
# include "str.h"

// return length of a string
int str_len(const char* const s) {
  int i=0; while(s[i++]); return(--i); }

// return 1 if s1 and s2 are the same string, 0 otherwise 
int sam(const char * const s1, const char * const s2) {
  int i=0;
  while(s2[i]==s1[i] && s1[i]!='\0' && s2[i]!='\0') i++;
  if(s1[i]=='\0' && s2[i]=='\0') return 1;
  return 0; }

// return 1 if s1 is the begin of s2, 0 otherwise 
int bgn(const char * const s1, const char * const s2) {
  int i=0;
  while(s2[i]==s1[i] && s1[i]!='\0' && s2[i]!='\0') i++;
  if(s1[i]=='\0') return 1;
  return 0; }

// dump/replace string
char *str_dup(const char* const s) {
  char *r = malloc((len(s)+1) * sizeof(char));
  int i=0; do r[i]=s[i]; while(s[i++]);
  return(r); }

// inplace remove all instances of character from string
void rmchar(char *s, const char c) {
  int i = 0; // original positions
  int j = 0; // new positions
  int l = len(s);
  while(i<l) {
    if(s[i] == c) { i++; continue; }	// skip c
    if(i == j) { i++; j++; continue; }	// advance both, they are the same
    s[j++] = s[i++]; }			// copy down
  s[j] = 0; }				// end

