// color.c: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// terminal colors support

# include "str.h"

// inplace remove all escape sequences from string
void rmesc(char *s) {
  int i = 0; // original positions
  int j = 0; // new positions
  int l = len(s);
  while(i<l) {
    if(i+4<l && s[i]==033 && s[i+1]=='[' && s[i+2]>='0' && s[i+2]<='9' && s[i+3]=='m') { i+=4; continue; } // Esc[0m
    if(i+5<l && s[i]==033 && s[i+1]=='[' && s[i+2]>='0' && s[i+2]<='9' && s[i+3]>='0' && s[i+3]<='9' && s[i+4]=='m') { i+=5; continue; } // Esc[36m
    if(i == j) { i++; j++; continue; }	// advance both, they are the same
    s[j++] = s[i++]; }			// copy down
  s[j] = 0; }				// end

// compute length of string, excluding escape sequences
int esclen(const char *s) {
  int len = 0;
  for(int i=0; s[i]; i++) {
    if(s[i]==033 && s[i+1]!=0 && s[i+2]!=0 && s[i+3]!=0 && s[i+1]=='[' && s[i+2]>='0' && s[i+2]<='9' && s[i+3]=='m') { i+=3; continue; } // Esc[0m
    if(s[i]==033 && s[i+1]!=0 && s[i+2]!=0 && s[i+3]!=0 && s[i+4]!=0 && s[i+1]=='[' && s[i+2]>='0' && s[i+2]<='9' && s[i+3]>='0' && s[i+3]<='9' && s[i+4]=='m') { i+=4; continue; } // Esc[36m
    len++; }
  return len; }

// compute length of 1st line of string, excluding escape sequences
int esc1len(const char *s) {
  int len = 0;
  for(int i=0; s[i]; i++) {
    if(s[i]==033 && s[i+1]!=0 && s[i+2]!=0 && s[i+3]!=0 && s[i+1]=='[' && s[i+2]>='0' && s[i+2]<='9' && s[i+3]=='m') { i+=3; continue; } // Esc[0m
    if(s[i]==033 && s[i+1]!=0 && s[i+2]!=0 && s[i+3]!=0 && s[i+4]!=0 && s[i+1]=='[' && s[i+2]>='0' && s[i+2]<='9' && s[i+3]>='0' && s[i+3]<='9' && s[i+4]=='m') { i+=4; continue; } // Esc[36m
    if(s[i]=='\n') break;
    len++; }
  return len; }

