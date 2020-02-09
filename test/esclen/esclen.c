# include "color.h"
# include <string.h>
# include <stdlib.h>

int main(void) {

  char s[1024]; sprintf(s,"1"CD_"2"CG_"3"CR_"4"CD_"567\n");
  char *z = strdup(s); rmesc(z);
  fprintf(stderr,"--> %s %ld %d\n",s,strlen(s),esclen(s));
  fprintf(stderr,"==> %s %ld %d\n",z,strlen(z),esclen(z));

}
