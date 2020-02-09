# include <stdlib.h>
# include <stdio.h>
# include "fmt.h"

int main(void) {

  // dynamic
  char *s;
  printf("%s\n",s=f2s(0,1022000));
  free(s);

  // static
  char z[32];
  printf("%s\n",f2s(z,1.022000));
  printf("%s\n",f2s(z,1.026000,2));
  printf("%s\n",f2s(z,1.0000,1)); }

