// regex.c: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// regex support

# include <stdlib.h>
# include <regex.h>
# include <stdio.h>
# include "str.h"

// regex match, returns addres of whole match or 0
int match(const char *s, const char *pat) {
  regex_t rx;
  if(regcomp(&rx,pat,REG_EXTENDED|REG_NOSUB) != 0) return 0; // wrong regex
  int ok = regexec(&rx,s,0,0,0);
  regfree(&rx);
  if(ok!=0) return 0; // no match
  return 1; }

// replace pat by rep and return 1 if new replacement is possible
int replace(char *s, const int size, const char *pat, const char *rep) {

  // compile regex
  regex_t rx;
  if(regcomp(&rx,pat,REG_EXTENDED) != 0) return 0; // wrong regex

  // match regex
  regmatch_t *regmatch = malloc(sizeof(regmatch_t));
  int ok = regexec(&rx,s,1,regmatch,0);
  if(ok!=0) { free(regmatch); regfree(&rx); return 0; } // no match

  // replace string
  char *s1 = sdup(s); s1[regmatch[0].rm_so]=0;
  char *s2 = sdup(s+regmatch[0].rm_eo);
  snprintf(s,size,"%s%s%s",s1,rep,s2);
  free(regmatch);
  free(s1);
  free(s2);

  // check next match
  ok = regexec(&rx,s,0,0,0);
  regfree(&rx);
  if(ok!=0) return 0; // no other match passible
  return 1; }

